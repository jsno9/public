#! usr/bin/python
#coding=utf-8
#import urllib2
import sys
from jira import JIRA
import os
import re
import sys
import getpass
import urllib3


def jira_login():
    #print "Please input your jira account:"
    user = raw_input('Please input your jira account:')
    password = getpass.getpass("Please input your jira password:")
    # Connect to JIRA server
    try:
        jira = JIRA(server='https://jira.***.com', basic_auth=(user,password))
    except Exception as e:
        sys.exit('jira_login: error connect server {}'.format(e))
    
    return jira
    
def is_findstring(fullstring,string):
    if fullstring.find(string) == -1:
        return False
    else:
        return True

def is_releasebin(partfilename):
    fileformat=partfilename[-1]
    if is_findstring(fileformat,"bin")==False and is_findstring(fileformat,"dll")==False:
        return False
    fileformat=partfilename[-2]
    if fileformat[0]=='[' and fileformat[-1]==']':
        return True
    return False


def is_findplatform(partfilename,platform):
    fileformat=partfilename[-2]
    if is_findstring(fileformat,platform)!=False:
        return True
    return False

    
def getfiledate(partfilename):
    fileformat=partfilename[-2]
    data=fileformat
    filedate=data[-7:-2]
    return filedate

def getfwname(partfilename):
    fileformat=partfilename[-2]
    tmpdata=fileformat.split('_')
    data=tmpdata[0]
    fwname=data[1:-1]
    return fwname
    
def get_newest_attachment(jira,jira_issue,platform):
    issue = jira.issue(jira_issue)
    #return attachment
    #attachment = issue.fields.attachment[0]
    maxfiledate=0
    fwfiledict={}
    fwattachmentdict={}
    for attachment in issue.fields.attachment:
        filename=attachment.filename
        partfilename=filename.split('.')
        if is_releasebin(partfilename)==True and is_findplatform(partfilename,platform)==True:
            filedate=getfiledate(partfilename)
            fwname=getfwname(partfilename)
            if fwfiledict.has_key(fwname):
                if filedate>fwfiledict[fwname]:
                    fwfiledict[fwname]=filedate
                    fwattachmentdict[fwname]=attachment
            else:
                maxfiledate=maxfiledate+1
                fwfiledict[fwname]=filedate;
                fwattachmentdict[fwname]=attachment
        else:
            continue
    if maxfiledate==0:
        #print ("error : can not find newest {interface} fw in jira {jira_issue}".format(interface=interface,jira_issue=jira_issue))
        sys.exit("error : can not find  fw in jira {jira_issue}".format(jira_issue=jira_issue))
    return fwattachmentdict

def save_attachment(path,attachment):
    filename=path+'/'+attachment.filename
    print filename
    fo = open(filename, "w")
    image=attachment.get()
    fo.write(image);
    fo.close
    
if __name__ == '__main__':
    urllib3.disable_warnings()
    jira=jira_login();
    #attachment = issue.fields.attachment[0]
    print sys.argv[1]
    print sys.argv[2]
    jira_issue=sys.argv[1]
    dir=sys.argv[3]
    platform=sys.argv[2]
    attachmentdict=get_newest_attachment(jira,jira_issue,platform)
    for key,attachment in attachmentdict.items():
        save_attachment(dir,attachment)
    
    

