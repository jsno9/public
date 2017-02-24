# /usr/bin/python3.
# -*- coding: utf-8 -*-
from prettytable import PrettyTable
import itchat
from itchat.content import TEXT


ptable = PrettyTable()
header = '车次 车站 时间 历时 一等 二等 软卧 硬卧 硬座 无座'.split()
ptable._set_field_names(header)

print ptable
a = 'hello word'
@itchat.msg_register(itchat.content.TEXT)
def a_reply(msg):
	print msg['Text']
	string = msg['Text']
	string = string.split()
	print string[0]
	print string[1]
	print string[2]
	itchat.send(a, msg['FromUserName'])

itchat.auto_login(hotReload=True)
itchat.run()
