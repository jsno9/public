#! usr/bin/python
#coding=utf-8
import sys
from jira import JIRA
import os
import re
import sys
import getpass
import urllib3
JIRA_ISSUE_SUMMARY = r'***'
JIRA_ISSUE_DESCRIPTION = r'''***'''
JIRA_ISSUE_GROUP_WATCHER = [{'name': '***'}]
JIRA_PROJECT_KEY = r'***'

def jira_login():
    #print "Please input your jira account:"
    user = raw_input('Please input your jira account:')
    password = getpass.getpass("Please input your jira password:")
    print user
    print password
    # Connect to JIRA server
    try:
        jira = JIRA(server='https://jira.***.com', basic_auth=(user,password))
    except Exception as e:
        sys.exit('jira_login: error connect server {}'.format(e))
    
    return (jira,user)

if __name__ == '__main__':
    file=sys.argv[1]
    urllib3.disable_warnings()
    (jira,user)=jira_login()
    JIRA_ISSUE_ASSIGNEE=user
    print JIRA_ISSUE_ASSIGNEE
    print len(sys.argv)
    
    subissue_dict = {
        'project': {'key': JIRA_PROJECT_KEY},
        'summary': JIRA_ISSUE_SUMMARY,
        'description': JIRA_ISSUE_DESCRIPTION,
        'issuetype': {'name': 'Task'},
        'assignee': {'name': JIRA_ISSUE_ASSIGNEE},
        'customfield_10601': [{'value': 'General'}], # RTL NO
        'customfield_10602': [{'value': 'BT UART'},{'value': 'BT USB'}], # Chip Type
        'customfield_10603': {'value': 'MAC_FW'},  # Function
        'customfield_10604': {'value': 'General'}, # OS
        'customfield_10605': {'value': 'Tablet'}, # Application
        'customfield_10606': [{'value': 'Others'}], # Customer
    }
    try:
        subtask_issue = jira.create_issue(fields=subissue_dict)
    except Exception as e:
        jira.kill_session()
        sys.exit('jira_push_tool: error {}'.format(e))
    
    
    try:
        for i in range(1,len(sys.argv)):
            attachment = jira.add_attachment(issue=subtask_issue, attachment=sys.argv[i])
    except Exception as e:
        jira.kill_session()
        sys.exit('jira_push_tool: error {}'.format(e))
        
        
