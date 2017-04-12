# -*- coding: utf-8 -*-
import xdrlib
import xlrd
import sys

def open_excel(file='file.xls'):
	try:
		data = xlrd.open_workbook(file)
		return data
	except Exception,e:
		print str(e)

def excel_table_byindex(file='file.xls',colnameindex=0,by_index=0):
	data = open_excel(file)
	table = data.sheets()[by_index]
	nrows = table.nrows
	print nrows
	ncols = table.ncols
	print ncols
	colnames = table.row_values(colnameindex)
	print "colnames",colnames
	list =[]
	for rownum in range(1,nrows):
		row = table.row_values(rownum)
		print row
		if row:
			app = {}
			for i in range(len(colnames)):
				app[colnames[i]] = row[i]
			list.append(app)
	return list	

def excel_read_cell(file='file.xls',by_index=0,nrow=0,ncol=0):
	data = open_excel(file)
	table = data.sheets()[by_index]
	cell = table.cell_value(nrow,ncol)
	return cell

def excel_row_col(file='file.xls',by_index=0):
	data = open_excel(file)
	table = data.sheets()[by_index]
	nrows = table.nrows
	ncols = table.ncols
	list =[]
	list.append(nrows)
	list.append(ncols)
	return list

def find_string(file='file.xls',string='haha',by_index=0,arow=0,acol=0):
	pathflag=0
	for i in range(0,arow):
		for j in range(0,acol):
			data = excel_read_cell('thermal1.xls',by_index=1,nrow=i,ncol=j)
			if data==string:
				row = i
				col = j
				pathflag = 1
				break
#			elif data=='':
#				print 'null',i,j
		if pathflag==1:
			break
	list =[]
	list.append(row)
	list.append(col)
	return list

def main():
	list = excel_row_col('thermal1.xls',by_index=1)
	arows = list[0]
	acols = list[1]
	list = find_string('thermal1.xls',string='Path',by_index=1,arow=arows,acol=acols)
	print list
	nrow = list[0]
	ncol = list[1]

	thermalcount=0
	fp=open('thermalconfig.txt','w')
	for i in range(nrow+1,arows):
		data = excel_read_cell('thermal1.xls',by_index=1,nrow=i,ncol=ncol)
		if data =='':
			break
		if data[11]=='T' and data[12]=='h' and data[13]=='e' and data[14]=='r' and data[15]=='m' and data[16]=='a' and data[17]=='l': 
			thermalcount=thermalcount+1
		fp.write(data+'\n')
		data = excel_read_cell('thermal1.xls',by_index=1,nrow=i,ncol=ncol-2)
		fp.write(data+'\n')

	fp.write("/data/data/ThermalCount"+'\n')
	thermalcountstr=str(thermalcount)
	fp.write(thermalcountstr+'\n')
	fp.write("end")
		
	
	


if __name__=="__main__":
	main()





