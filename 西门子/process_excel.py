# -*- coding: utf-8 -*-
import os

import xlrd
import xlwt

#单元格格式
style = xlwt.Style.easyxf("font:name 宋体,height 220")
style_muban = xlwt.Style.easyxf("font:name 等线,height 220,bold on;align:horiz center")
style_muban_table =  xlwt.Style.easyxf("font:name 等线,height 220,bold on;align:horiz center;borders:top 1,bottom 1,left 1,right 1")

file_name_list = []

#新建一个excel文件
file_result = xlwt.Workbook() #注意这里的Workbook首字母是大写，无语吧
count_ok_in_all_device = 0

result_path = os.getcwd() + '\西门子'
muban_file = xlrd.open_workbook(result_path + "\\muban.xlsx")
muban_sheet = muban_file.sheets()[0]

data_path = os.getcwd() + '\西门子\data'
for file in os.listdir(data_path):
    # if not os.path.isdir(file_path):
    if os.path.splitext(file)[1]=='.xlsx' or os.path.splitext(file)[1]=='.xls':
        print("读取到excel文件："+file)
        file_path = os.path.join(data_path, file)
        file_name_list.append(file_path)
for file_path in file_name_list:
    data_file = xlrd.open_workbook(file_path)
    count_ok = 0
    for data_sheet in data_file.sheets():
        print ('正在处理：'+file_path.replace(data_path,"")+'：'+data_sheet.name)
        #获取行数
        nrows = data_sheet.nrows
        #获取列数
        ncols = data_sheet.ncols
        # print(nrows)
        # print(ncols)
        device_state = data_sheet.cell(0,7).value
        # print(device_state)
        if(device_state == "合格"):
            # print("产品合格")
            count_ok = count_ok +1
            #新建一个sheet
            table_write = file_result.add_sheet(str(count_ok_in_all_device+1))
            count_ok_in_all_device = count_ok_in_all_device + 1
            #复制模板   
            table_write.write(0,0,muban_sheet.cell(0,0).value,style_muban)
            table_write.write(0,1,muban_sheet.cell(0,1).value,style_muban)
            table_write.write(0,2,muban_sheet.cell(0,2).value,style_muban)
            table_write.write_merge(2,2,4,7,muban_sheet.cell(2,4).value,style_muban_table)
            
            for i in {3,7,8}:
                for j in range(4,8):
                    table_write.write(i,j,muban_sheet.cell(i,j).value,style_muban_table)
                
            table_write.write(4,4,muban_sheet.cell(4,j).value,style_muban_table)
            table_write.write(4,5,xlwt.Formula("MAX(A2:A1800)"),style_muban_table)
            table_write.write(4,6,xlwt.Formula("MAX(B2:B1800)"),style_muban_table)
            table_write.write(4,7,xlwt.Formula("MAX(C2:C1800)"),style_muban_table)

            table_write.write(5,4,muban_sheet.cell(5,j).value,style_muban_table)
            table_write.write(5,5,xlwt.Formula("MIN(A2:A1800)"),style_muban_table)
            table_write.write(5,6,xlwt.Formula("MIN(B2:B1800)"),style_muban_table)
            table_write.write(5,7,xlwt.Formula("MIN(C2:C1800)"),style_muban_table)

            table_write.write(6,4,muban_sheet.cell(6,j).value,style_muban_table)
            table_write.write(6,5,xlwt.Formula("F5-F6"),style_muban_table)
            table_write.write(6,6,xlwt.Formula("G5-G6"),style_muban_table)
            table_write.write(6,7,xlwt.Formula("H5-H6"),style_muban_table)

            
            #复制数据
            for i in range(5,nrows):
                table_write.write(i-4,0,data_sheet.cell(i,1).value,style)
                table_write.write(i-4,1,data_sheet.cell(i,3).value,style)
                table_write.write(i-4,2,data_sheet.cell(i,5).value,style)
                
    print("本表格中共有"+str(count_ok)+"个设备合格")
    file_result.save("Result.xls")

print("结果保存完毕")

