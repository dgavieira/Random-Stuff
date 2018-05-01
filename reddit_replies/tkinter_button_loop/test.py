#!/usr/bin/env python2
from Tkinter import *
import datetime

this_year = datetime.datetime.now().year

class TxLedGui:
    def __init__(self, master):
        self.master = master

        master.title("TxLED Reporting")

        # Date range selection in the form of quarterly options and year entry
        lbl = Label(master, text="Quarter:", width=10, anchor=W)
        lbl.grid(row=0, column=0, sticky=W)

        self.quarter = StringVar(master)
        self.quarter.set("Q1 (Jan 1 - Mar 31)")

        self.quarter_options = ["Q1 (Jan 1 - Mar 31)", "Q2 (Apr 1 - Jun 30)",
                                "Q3 (Jul 1 - Sep 30)", "Q4 (Oct 1 - Dec 31)"]

        self.quarter_select = OptionMenu(master, self.quarter, *self.quarter_options)
        self.quarter_select.grid(row=1, column=0, sticky=W)

        lbl = Label(master, text="Year:", width=10, anchor=W)
        lbl.grid(row=0, column=1, sticky=W)

        self.year_entry = Entry(master, width=10)
        self.year_entry.insert(END, this_year)
        self.year_entry.grid(row=1, column=1, sticky=W)
        # End date range selection

        # DB Credentials
        lbl = Label(master, text="DB User:", width=10, anchor=W)
        lbl.grid(row=2, column=0, sticky=W)

        lbl = Label(master, text="DB Pass:", width=10, anchor=W)
        lbl.grid(row=2, column=1, sticky=W)

        self.user_entry = Entry(master, width=10)
        self.user_entry.grid(row=3, column=0, sticky=W)

        self.pass_entry = Entry(master, width=10, show='*')
        self.pass_entry.grid(row=3, column=1, sticky=W)
        # End DB Creds

        # Button to start process
        btn = Button(master, text="Generate Report", command=self.gen_report, width=15)
        btn.grid(row=4, column=0, sticky=W)

    def gen_report(self):
        print "working"

root = Tk()
txledgui = TxLedGui(root)
root.mainloop()
