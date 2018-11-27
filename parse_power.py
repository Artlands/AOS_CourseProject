#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import json
import numpy as np
import matplotlib.pyplot as plt

class ParsePower:

    def __init__(self):
        self.parse_args(sys.argv)

    @staticmethod
    def run():
        parser = ParsePower()
        parser.parse()

    def parse(self):

        print (f"Parse Power Data in Watts...")

        raw_power = []
        dmc_power = []
        power_eff = []
        raw_total = 0
        dmc_total = 0

        label = ["LINK_PHY_POWER", "LINK_LOCAL_ROUTE_POWER", "LINK_REMOTE_ROUTE_POWER",
        "XBAR_RQST_SLOT_POWER", "XBAR_RSP_SLOT_POWER","VAULT_RQST_SLOT_POWER",
        "VAULT_RSP_SLOT_POWER", "VAULT_CTRL_POWER", "ROW_ACCESS_POWER"]

        label2 = ["LINK_PHY", "LINK_LOCAL_ROUTE", "LINK_REMOTE_ROUTE",
        "XBAR_RQST_SLOT", "XBAR_RSP_SLOT","VAULT_RQST_SLOT",
        "VAULT_RSP_SLOT", "VAULT_CTRL", "ROW_ACCESS"]

        with open(self.raw_file_name, "r") as rawfile:
            line = rawfile.readline()
            while line:
                if ( line[:12] == "HMCSIM_TRACE" and float(line.split(' : ')[3].rstrip()) != 0):
                    raw_power.append(float(line.split(' : ')[3].rstrip())/1000)
                line = rawfile.readline()

        with open(self.dmc_file_name, "r") as dmcfile:
            line = dmcfile.readline()
            while line:
                if ( line[:12] == "HMCSIM_TRACE" and float(line.split(' : ')[3].rstrip()) != 0):
                    dmc_power.append(float(line.split(' : ')[3].rstrip())/1000)
                line = dmcfile.readline()

        for power in raw_power:
            raw_total += power
        for power in dmc_power:
            dmc_total += power

        for i in range(len(raw_power)):
            efficiency = (raw_power[i] - dmc_power[i])/raw_power[i]
            power_eff.append(efficiency * 100)


        print(f"RAW POWER : {raw_power}, TOTAL : {raw_total}")
        print(f"DMC POWER : {dmc_power}, TOTAL : {dmc_total}")
        print(f"POWER EFFICIENCY : {power_eff}, TOTAL : {(raw_total - dmc_total)/raw_total}")

        # raw_data = tuple(raw_power)
        # dmc_data = tuple(dmc_power)
        eff_data = tuple(power_eff)
        xlabels = tuple(label2)

        ind = np.arange(len(eff_data))
        width = 0.35

        fig, ax = plt.subplots()
        rects1 = ax.bar(ind - width/2, eff_data, width, color = 'SkyBlue', label = "Raw Trace")

        # Add some text for labels, title and custom x-axis tick labels, etc.
        ax.set_ylabel('Efficiency')
        ax.set_title('Power Consumption Efficiency')
        ax.set_xticks(ind)
        ax.set_xticklabels(xlabels)
        ax.legend()
        plt.xticks(rotation=90)

        plt.show()


    def parse_args(self,argv):
        """parse args and set up instance variables"""
        try:
            self.raw_file_name = argv[1]
            self.dmc_file_name = argv[2]
            self.working_dir = os.getcwd()
        except:
            print (self.usage())
            sys.exit(1)

    def usage(self):
        return """
        Parse Power data and plot results:

        Usage:

            $ python parse_power.py <raw_trace_file_name> <dmc_trace_file_name>

        """

if __name__ == "__main__":
    ParsePower.run()
