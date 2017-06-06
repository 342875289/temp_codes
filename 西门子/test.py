# -*- coding: utf-8 -*-
import os

import xlwt
from xlutils.copy import copy
import xlrd 

filename = 'VG300-485'
#filename = 'test'

rb = xlrd.open_workbook(filename+'.xls',formatting_info=True, on_demand=True)
wb = copy(rb)
print(len(rb.sheet_names()))
for i in range(  len(  rb.sheet_names()  ) ) :
    wb.get_sheet(i).name = str(i+1)

wb.save(filename+'3.xls')
print("OK")