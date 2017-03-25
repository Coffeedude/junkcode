import sys, os, getopt, csv

def print_usage(command):
    print "Usage: {0} --roles=<file> --updates=<file> --boards=<file> [--report=<ReportName>]".format(command)
    print "    --roles    [default: ./Users_and_their_Board_Roles.csv]"
    print "    --updates  [default: ./Most_Recent_Write_Activity_for_each_active_user.csv"
    print "    --boards   [default: ./MostRecentActivityPerUserAndBoard.csv"
    print "    --report   [default: Readers]  Report name to print to stdout"
    print "               Valid report names are: Readers, Accounts, Stale"
    

def print_report_readers(accts):
    for k in accts.keys():
        if len(accts[k]['boards'].keys()) == 0:
            print k

def print_report_all_accounts(accts):
    for k in sorted(accts.keys()):
        print "Account: {0}".format(k)
        if len(accts[k]['boards']) == 0:
            print "    Boards: None"
        else:
            for b in accts[k]['boards']:
                print "    \"{0}\" (Role: \"{1}\"; LastWrite: \"{2}\")".format(
                        b,
                        accts[k]['boards'][b]['role'],
                        accts[k]['boards'][b]['last_write'])

def print_report_stale(accts):
    for k in accts.keys():
        if accts[k]['last_write'] == "":
            print k

##
## MAIN
##
optlist, args = getopt.getopt(sys.argv[1:], '',
                              ['roles=', 
                               'updates=', 
                               'boards=',
                               'report=',
                               'help'])

options = {}
for k,v in optlist:
    options[k] = v

if options.has_key('--help'):
    print_usage(sys.argv[0])
    sys.exit(1)

if not (options.has_key('--roles') and options.has_key('--updates') and options.has_key('--boards')):
    print_usage(sys.argv[0])
    sys.exit(1)

if not options.has_key('--report'):
    options['--report'] = "Readers"

f_roles = open(options['--roles'])
f_boards = open(options['--boards'])
f_updates = open(options['--updates'])


# UserName,LastName,FirstName,CreationDate,BoardTitle,IsOrganizationAdmin,BoardRole
roles = csv.DictReader(f_roles)
# UserName,LastName,FirstName,CreationDate,MostRecentWriteOrUpdateActivityUTCTime
updates = csv.DictReader(f_updates)
# BoardTitle,UserName,LastName,FirstName,CreationDate,IsOrganizationAdmin,BoardRole,MostRecentWriteActivityUTCTime
boards = csv.DictReader(f_boards)

accounts = {}
for row in updates:
   accounts[row['UserName']] = {}
   accounts[row['UserName']]['created'] = row['CreationDate']
   accounts[row['UserName']]['last_write'] = row['MostRecentWriteOrUpdateActivityUTCTime']
   accounts[row['UserName']]['boards'] = {}

for row in roles: 
    accounts[row['UserName']]['boards'][row['BoardTitle']] = {}
    if len(row['BoardRole']) == 0:
        accounts[row['UserName']]['boards'][row['BoardTitle']]['role'] = 'Creator'
    else:
        accounts[row['UserName']]['boards'][row['BoardTitle']]['role'] = row['BoardRole']

for row in boards:
    accounts[row['UserName']]['boards'][row['BoardTitle']]['last_write'] = row['MostRecentWriteActivityUTCTime']

acct_readers = []
acct_no_writes = []


if options['--report'] == "Readers":
    print_report_readers(accounts)
    
elif options['--report'] == "Accounts":
    print_report_all_accounts(accounts)        

elif options['--report'] == "Stale":
    print_report_stale(accounts)        

else:
    print "Unknown Report name {0}!  See --help".format(options['--report'])

