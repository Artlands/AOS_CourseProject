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
        raw_rate = []

        dmc_power = []
        dmc_rate = []

        power_eff = []
        raw_total = 0
        dmc_total = 0

        label = ["LINK-PHY", "LINK-LOCAL-ROUTE", "LINK-REMOTE-ROUTE",
        "XBAR-RQST-SLOT", "XBAR-RSP-SLOT","VAULT-RQST-SLOT",
        "VAULT-RSP-SLOT", "VAULT-CTRL", "ROW-ACCESS", "TOTAL"]

        plot_file_name = f"to_plot_stack.txt"

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

        for i in range(len(raw_power)):
            rate = raw_power[i]/raw_total
            raw_rate.append(rate * 100)

        raw_rate.append(100);

        for i in range(len(dmc_power)):
            rate = dmc_power[i]/dmc_total
            dmc_rate.append(rate * 100)

        dmc_rate.append(100);

        power_eff.append(100 * (raw_total - dmc_total)/raw_total)

        with open(plot_file_name, "w") as plotfile:
            plotfile.write("\"CATEGORY\"   \"POWER SAVING RATE\"  \"RAW DATA POWER RATE\"   \"DMC DATA POWER RATE\" ")
            plotfile.write("\n")
            for i in range(len(label)):
                plotfile.write(f"{label[i]}  {power_eff[i]:.2f}  {raw_rate[i]:.2f}  {dmc_rate[i]:.2f}")
                plotfile.write("\n")

            ## for stack ##
            plotfile.write("\"CATEGORY\"   ")
            for i in range(len(label)):
                plotfile.write(f"\"{label[i]}\"   ")
            plotfile.write("\n")

            ## for stack power rate##
            # plotfile.write("\"RAW TRACE\"   ")
            # for i in range(len(raw_rate)):
            #     plotfile.write(f"{raw_rate[i]:.2f}   ")
            # plotfile.write("\n")
            #
            # plotfile.write("\"DMC TRACE\"   ")
            # for i in range(len(dmc_rate)):
            #     plotfile.write(f"{dmc_rate[i]:.2f}   ")
            # plotfile.write("\n")
            ## for stack power##
            # plotfile.write("\"RAW TRACE\"   ")
            # for i in range(len(raw_power)):
            #     plotfile.write(f"{raw_power[i]:.2f}   ")
            # plotfile.write(f"{raw_total:.2f}   ")
            # plotfile.write("\n")
            #
            # plotfile.write("\"DMC TRACE\"   ")
            # for i in range(len(dmc_power)):
            #     plotfile.write(f"{dmc_power[i]:.2f}   ")
            # plotfile.write(f"{dmc_total:.2f}   ")
            # plotfile.write("\n")



        print(label)
        print(f"RAW POWER : {raw_power}, TOTAL : {raw_total}")
        print(f"RAW POWER RATE : {raw_rate}")

        print(f"DMC POWER : {dmc_power}, TOTAL : {dmc_total}")
        print(f"DMC POWER RATE : {dmc_rate}")

        print(f"POWER SAVING RATES : {power_eff}, TOTAL : {100 * (raw_total - dmc_total)/raw_total}")

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
