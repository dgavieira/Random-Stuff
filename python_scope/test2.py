#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
A simple case.

The variable does exist in global scope.
"""

variable = 0

try:
    print('global: variable=%d' % variable)
except NameError:
    print("global: variable isn't defined")
