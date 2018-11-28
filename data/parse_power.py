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

        label = ["LINK-PHY", "LINK-LOCAL-ROUTE", "LINK-REMOTE-ROUTE",
        "XBAR-RQST-SLOT", "XBAR-RSP-SLOT","VAULT-RQST-SLOT",
        "VAULT-RSP-SLOT", "VAULT-CTRL", "ROW-ACCESS", "TOTAL"]

        plot_file_name = f"to_plot.txt"

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
                    dmc_power.append(float(line.split(' : ')[3].rstrip())/1000:)
                line = dmcfile.readline()

        for power in raw_power:
            raw_total += power
        for power in dmc_power:
            dmc_total += power

        for i in range(len(raw_power)):
            efficiency = (raw_power[i] - dmc_power[i])/raw_power[i]
            power_eff.append(efficiency * 100)

        power_eff.append(100 * (raw_total - dmc_total)/raw_total)

        with open(plot_file_name, "w") as plotfile:
            for i in range(len(label)):
                plotfile.write(f"{label[i]}   {power_eff[i]:.2f}")
                plotfile.write("\n")

        print(label)
        print(f"RAW POWER : {raw_power}, TOTAL : {raw_total}")
        print(f"DMC POWER : {dmc_power}, TOTAL : {dmc_total}")
        print(f"POWER EFFICIENCY : {power_eff}, TOTAL : {100 * (raw_total - dmc_total)/raw_total}")

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
