# /usr/bin/python3.
# -*- coding: utf-8 -*-
import itchat
from itchat.content import TEXT


"""命令行火车票查看器

Usage:
    tickets [-gdtkz] <from> <to> <date>

Options:
    -h,--help   显示帮助菜单
    -g          高铁
    -d          动车
    -t          特快
    -k          快速
    -z          直达

Example:
    tickets 北京 上海 2016-10-10
    tickets -dg 成都 南京 2016-10-10
"""

def cli(from_station,to_station,date):
	from docopt import docopt
	from stations import stations
	import get_data
	import formats
	
	arguments = {'-d': False,'-g': False,'-k': False,'-t': False,'-z': False,'<date>': '2017-03-03','<from>': 'shanghai','<to>': 'beijing'}
	#arguments = docopt(__doc__)
	arguments['<from>'] = from_station
	from_station = stations.get(from_station)
	arguments['<to>'] = to_station
	to_station = stations.get(to_station)
	#date = arguments['<date>']
	arguments['<date>'] = date
	options = ''.join([
		key for key, value in arguments.items() if value is True
	])
	#print(options)
	print from_station
	print to_station
	print date
	get_data = get_data.GetData(date, from_station, to_station)
	result = get_data.request_12306()
	#print result
	#print result
	if result['httpstatus'] == 200 and result.get('data') is not None and result.get('data') != '':
		format_result = formats.Formats(result.get('data'), options)
		return format_result.myformat_data()
	else:
		return result['messages']

def clitrainnum(trainnum,from_station,to_station,date):
	from docopt import docopt
	from stations import stations
	import get_data
	import formats
	
	arguments = {'-d': False,'-g': False,'-k': False,'-t': False,'-z': False,'<date>': '2017-03-03','<from>': 'shanghai','<to>': 'beijing'}
	#arguments = docopt(__doc__)
	arguments['<from>'] = from_station
	from_station = stations.get(from_station)
	arguments['<to>'] = to_station
	to_station = stations.get(to_station)
	#date = arguments['<date>']
	arguments['<date>'] = date
	options = ''.join([
		key for key, value in arguments.items() if value is True
	])
	#print(options)
	print from_station
	print to_station
	print date
	get_data = get_data.GetData(date, from_station, to_station)
	result = get_data.request_12306()
	#print result
	#print result
	if result['httpstatus'] == 200 and result.get('data') is not None and result.get('data') != '':
		format_result = formats.Formats(result.get('data'), options);
		return format_result.trainnumformat_data(trainnum);
	else:
		return result['messages']


@itchat.msg_register(itchat.content.TEXT)
def a_reply(msg):
	#print msg['Text']
	string = msg['Text']
	string = string.split()
	a=[]
	if string[0]!='' and string[1]!='' and string[2]!='':
		if string[0][0] == 'G' or string[0][0] == 'D' or string[0][0] == 'Z' or string[0][0] == 'K' or string[0][0] == 'T':
			a = clitrainnum(string[0],string[1],string[2],string[3])
			#print a
			if a=='No Train':
				itchat.send(a, msg['FromUserName'])
			else:
				itchat.send(a[0], msg['FromUserName'])
		else:
			a = cli(string[0],string[1],string[2])
			#print a
			for item in a:
				itchat.send(item, msg['FromUserName'])



if __name__ == '__main__':
	itchat.auto_login(hotReload=True)
	itchat.run()
	a_reply(msg)
	

#if __name__ == '__main__':
#    print(cli())



