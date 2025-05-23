#!/usr/bin/env python3
import os
import logging
import csv

from .parse_csv import *

ota_keys = [
    'strategy',
    'app_security_counter'
]

class OTA(list):

    def __getitem__(self, key):
        return self.csv.dic[key]

    def get_app_security_counter(self):
        return int(self.csv.dic['app_security_counter'])

    def get_bl2_security_counter(self):
        return 4

    def get_strategy(self):
        return self.csv.dic['strategy']

    def is_overwrite(self):
        if(self.csv.dic['strategy'].upper() == 'OVERWRITE'):
            return True
        else:
            return False

    def __init__(self, csv_file):
        self.csv = Csv(csv_file, False, ota_keys)
