# /usr/bin/python3.
# -*- coding: utf-8 -*-

"""返回规范格式的 table"""
import sys  
reload(sys)  
sys.setdefaultencoding('utf8')  
#from search import search
from prettytable import PrettyTable
from colorama import init, Fore

class Formats(object):
	def __init__(self, datas, options):
		self.datas = datas
		self.options = options

	def format_data(self):
		"""返回规范的 table"""
		ptable = PrettyTable()
		header = '车次 车站 时间 历时 一等 二等 软卧 硬卧 硬座 无座'.split()
		ptable._set_field_names(header)
		for train in self.datas:
			trains = train.get('queryLeftNewDTO')
			#print trains
			if trains is not None and trains != '':
				initial = trains['station_train_code'].lower()[0]
				if not self.options or initial in self.options:
					ptable.add_row([trains['station_train_code'], Fore.GREEN + "%s -> %s" % (trains['from_station_name'], trains['to_station_name']) + Fore.RESET, Fore.RED + "%s -> %s" % (trains['start_time'], trains['arrive_time']) + Fore.RESET , trains['lishi'], trains['zy_num'], trains['ze_num'], trains['rw_num'], trains['yw_num'], trains['yz_num'], trains['wz_num']])
		return ptable

	def myformat_data(self):
		msg=[]
		print self.datas
		for train in self.datas:
			trains = train.get('queryLeftNewDTO')
			print trains
			if trains is not None and trains != '':
				msg.append(trains['station_train_code']+' '+trains['from_station_name']+'-'+trains['to_station_name']+' '+trains['start_time']+'-'+trains['arrive_time']+' '+'二等:'+trains['ze_num'])
		return msg

	def trainnumformat_data(self,trainnum):
		msg=[]
		for train in self.datas:
			trains = train.get('queryLeftNewDTO')
			#print trains
			if trains is not None and trains != '':
				if(trains['station_train_code'] == trainnum):
					msg.append(trains['station_train_code']+' '+trains['from_station_name']+'-'+trains['to_station_name']+' '+trains['start_time']+'-'+trains['arrive_time']+' '+'历时:'+trains['lishi']+''+'一等:'+trains['zy_num']+' '+'二等:'+trains['ze_num']+' '+'软卧:'+trains['rw_num']+' '+'硬卧:'+trains['yw_num']+' '+'硬座:'+trains['yz_num']+' '+'无座:'+trains['wz_num'])
					return msg
		return 'No Train'

#a_data = Formats(search.get('data'))
#print(a_data.format_data())
