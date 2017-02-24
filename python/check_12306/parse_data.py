# /usr/bin/python.
# -*- coding: utf-8 -*-



from pprint import pprint
import re
import requests


urls = 'https://kyfw.12306.cn/otn/leftTicket/query?leftTicketDTO.train_date=2017-02-22&leftTicketDTO.from_station=WHH&leftTicketDTO.to_station=NJH&purpose_codes=ADULT'
r = requests.get(urls, verify=False)
#stations = re.findall('*' , response.text)
#print r.text
a=r.json()
#print a
r = a.get('data')

for train in r:
	trains = train.get('queryLeftNewDTO')
	#print trains
#stations = dict(response)
#print r
pprint(dict(trains), indent=4)
