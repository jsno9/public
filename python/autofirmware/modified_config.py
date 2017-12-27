#! usr/bin/python
#coding=utf-8
import sys
import os
import re
import sys
import struct

def print_hex(filename):
    f = open(filename, "rb+")
    f.seek(0,0)  
    index=0  
    while True:  
        temp=f.read(1)
        if len(temp) == 0:  
            break  
        else:
            print "%3s" % temp.encode('hex'),  
            index=index+1  
        if index == 16:  
            index=0  
            print  
    print
    f.close

def findheader(filename,offsetnamehigh,offsetnamelow):
    #print "haha"
    #print hex(offsetnamehigh)
    #print hex(offsetnamelow)
    f = open(filename, "rb+")
    f.seek(0,0)
    #temp=f.read(1)
    #dataint=int(temp.encode('hex'),16)
    #datachar=chr(data)
    index=6
    length=0
    f.seek(index,0)

    while True:
        f.seek(index,0)
        temp=f.read(1)
        if len(temp) == 0:  
            break
        dataint=int(temp.encode('hex'),16)
        index=index+1
        if dataint==offsetnamelow:
            temp=f.read(1)
            if len(temp) == 0:  
                break
            dataint=int(temp.encode('hex'),16)
            index=index+1
            if dataint==offsetnamehigh:
                temp=f.read(1)
                if len(temp) == 0:  
                    break
                dataint=int(temp.encode('hex'),16)
                length=dataint
                index=index-2
                break;
            else:
                temp=f.read(1)
                if len(temp) == 0:  
                    break
                dataint=int(temp.encode('hex'),16)
                templength=dataint
                index=index+1
                index=index+templength
                continue
        else:
            temp=f.read(1)
            if len(temp) == 0:  
                break
            dataint=int(temp.encode('hex'),16)
            index=index+1
            temp=f.read(1)
            dataint=int(temp.encode('hex'),16)
            templength=dataint
            if len(temp) == 0:  
                break
            index=index+1
            index=index+templength
    f.close
    print length
    print index
    return (length,index)

def readoffsetbyte(filename,offset):
    f = open(filename, "rb+")
    f.seek(offset,0)
    temp=f.read(1)
    dataint=int(temp.encode('hex'),16)
    f.close
    return dataint
    
def readoffsetshort(filename,offset):
    f = open(filename, "rb+")
    f.seek(offset,0)
    temp=f.read(1)
    dataint=int(temp.encode('hex'),16)
    temp=f.read(1)
    dataint2=int(temp.encode('hex'),16)
    dataint=(dataint2<<8)+dataint
    f.close
    return dataint
    
def writeoffsetbyte(filename,offset,byte):
    #echo -n -e "\xfa" > temp2
    #dd if=temp2 of=rtl8723bstest_config bs=1 count=1 seek=56 conv=notrunc
    #os.environ['var'] = str(value)
    byte=hex(byte)
    byte=str(byte)
    byte=byte[1:4]
    byte="\\"+byte
    os.environ['data'] = str(byte)
    os.environ['seek'] = str(offset)
    os.environ['file'] = str(filename)
    os.system("echo -n -e $data > temp")
    os.system("dd if=temp of=$file bs=1 count=1 seek=$seek conv=notrunc")
    os.system("rm -rf temp")

def heartbeat_enable(filename,offset):
    data=readoffsetbyte(filename,offset+3)
    print hex(data)
    data=data|0x12
    print hex(data)
    writeoffsetbyte(filename,offset+3,data)
    
def heartbeat_disable(filename,offset):
    data=readoffsetbyte(filename,offset+3)
    print hex(data)
    data=data&0xec
    print hex(data)
    writeoffsetbyte(filename,offset+3,data)

def change_totallength(filename,length):
    hlen=(length>>8)&0xff
    llen=length&0xff
    writeoffsetbyte(filename,4,llen)
    writeoffsetbyte(filename,5,hlen)
    
def heartbeat_add(filename):
    data=readoffsetshort(filename,4)
    offset=6+data
    writeoffsetbyte(filename,offset,0x7a)
    writeoffsetbyte(filename,offset+1,0x01)
    writeoffsetbyte(filename,offset+2,0x02)
    writeoffsetbyte(filename,offset+3,0x12)
    writeoffsetbyte(filename,offset+4,0x19)
    length=data+5
    change_totallength(filename,length)
    
def change_heartbeat(filename,enable):
    (length,offset)=findheader(filename,0x01,0x7a)
    if length == 0:
        (length,offset)=findheader(filename,0x18,0xbe)
        if length == 0:
            if enable==1:
                heartbeat_add(filename)
            else:
                print "Tablet config does not need heartbeat!"
        else:
            if enable==1:
                heartbeat_enable(filename,offset)
            else:
                heartbeat_disable(filename,offset)
    else:
        if enable==1:
            heartbeat_enable(filename,offset)
        else:
            heartbeat_disable(filename,offset)

def flowcontrol_disable(filename,offset):
    data=readoffsetbyte(filename,offset+15)
    print hex(data)
    data=data&0xfb
    print hex(data)
    writeoffsetbyte(filename,offset+15,data)

def change_flowcontrol(filename):
    (length,offset)=findheader(filename,0x00,0x0c)
    if length == 0:  
        print "The config is error,please check 0x000c"
        exit(0);
    else:
        flowcontrol_disable(filename,offset)

def change_baudrate(filename):
    (length,offset)=findheader(filename,0x00,0x0c)
    if length == 0:  
        print "The config is error,please check 0x000c"
        exit(0);
    else:
        writeoffsetbyte(filename,offset+3,0x01)
        writeoffsetbyte(filename,offset+4,0x50)
        writeoffsetbyte(filename,offset+5,0x00)
        writeoffsetbyte(filename,offset+6,0x00)
    print "baudrate"

def modified_tv_config(filename):
    #print_hex(filename)
    change_heartbeat(filename,1)
    print "tv"
    
def modified_tablet_config(filename):
    #print_hex(filename)
    change_heartbeat(filename,0)
    print "tablet"
    
def modified_usb_config(filename):
    #print_hex(filename)
    print "usb"
    
def modified_uart_config(filename):
    change_flowcontrol(filename)
    change_baudrate(filename)
    print "uart"

if __name__ == '__main__':
    platform=sys.argv[1]
    interface=sys.argv[2]
    print sys.argv[3]
    #echo -n -e "\xfa" > temp2
    #dd if=temp2 of=rtl8723bstest_config bs=1 count=1 seek=56 conv=notrunc
    
    if platform=="tv":
        modified_tv_config(sys.argv[3]);
    else:
        modified_tablet_config(sys.argv[3]);
        
    if interface=="usb":
        modified_usb_config(sys.argv[3]);
    else:
        modified_uart_config(sys.argv[3]);


