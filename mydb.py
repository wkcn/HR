# -*- coding: utf-8 -*-
import sys
import MySQLdb
import time

#reload(sys)
#sys.setdefaultencoding('utf8')


conn = 0
cursor = 0
DBopened = 0

N = 7 #包括1000~1006
M = 256 #最大参赛队伍，缓存限制用

teamScore = [[[0,0,0,0] for t in range(N)] for i in range(M)] #分数记录 [startTime,endTime,useTime,penny]

def ConnectDB():
    global conn
    global cursor
    global DBopened
    
    DBopened = DBopened + 1
    if DBopened != 1:
        return
     
    # 连接数据库　
    try:
        db = 'elife'
        user = 'wkcn'
        pw = 'wk951022'
        host = 'localhost'
        port = 3306
        try:
            import sae.const
            db = sae.const.MYSQL_DB
            user = sae.const.MYSQL_USER
            pw = sae.const.MYSQL_PASS
            host = sae.const.MYSQL_HOST
            port = int(sae.const.MYSQL_PORT)
        except ImportError:
            pass
        conn = MySQLdb.connect(host=host,user=user,passwd=pw,db=db,port=port,charset='utf8')
        cursor = conn.cursor()
    except Exception, e:
        print e
        sys.exit()
        
def ExitDB():
    global cursor
    global conn
    global DBopened
    DBopened = DBopened - 1
    if DBopened != 0:
        return
        
    try:
        cursor.close()
        conn.close()
    except:
        pass

        
def GetPW(id,pos):
    return (id + 13) * (pos + 17) * 19 * 23 % 10573 
newTeams = '''    create table teams
    (
        id int unsigned not null auto_increment primary key,
        name char(32) not null,
        mate1 char(64),
        mate2 char(64),
        mate3 char(64),
        mate4 char(64),
        Tstate int,
        AC int,
        penny int,
        reward int,
        route text
    );
    '''
    
newDetail = '''create table site%ddetail
    (
        id int unsigned not null auto_increment primary key,
        content text,
        pic text,
        truth text,
        A char(128),
        B char(128),
        C char(128),
        D char(128)
    );
    '''

newScore = '''    create table site%dscore
    (
        id int unsigned not null primary key,
        startTime int,
        endTime int,
        useTime int,
        penny int
    );
    '''
nextPlace = '''    create table nextplace
    (
        route char(16) primary key,
        place text
    );
    '''
    
def NewSiteDB():
    ConnectDB()
    #new site detail and score
    sql = newTeams
    cursor.execute(sql)
    
    sql = nextPlace
    cursor.execute(sql)
    
    for i in range(1000,1000+N):
        sqld = newDetail % (i)
        sqls = newScore % (i)
        cursor.execute(sqld)
        cursor.execute(sqls)
        
    conn.commit()
    ExitDB()
 
def NewTeam(id,name,mate1,mate2,mate3,mate4,reward):
    ConnectDB()
    sql = "insert into teams(id,name,mate1,mate2,mate3,mate4,Tstate,AC,penny,reward) values (%d,'%s','%s','%s','%s','%s',0,0,0,%d)" % (id,name,mate1,mate2,mate3,mate4,reward)
    cursor.execute(sql)
    conn.commit()
    sql = "select id from teams where name='%s' and mate1='%s' and mate2='%s' and mate3='%s' and mate4='%s'" % (name,mate1,mate2,mate3,mate4) 
    cursor.execute(sql)
    alldata = cursor.fetchall()
    ExitDB()
    return (alldata[0][0])
    
def NewPlace(route,place):
    ConnectDB()
    sql = "insert into nextplace(route,place) values ('%s','%s')" % (route,place)
    cursor.execute(sql)
    conn.commit()
    ExitDB()
    
def NewQues(pos,content,pic,truth,A='',B='',C='',D=''):
    ConnectDB()
    sql = "insert into site%ddetail(content,pic,truth,A,B,C,D) values ('%s','%s','%s','%s','%s','%s','%s')" % (pos+1000,content,pic,truth,A,B,C,D)
    cursor.execute(sql)
    conn.commit()
    ExitDB()
 

 
def DelDB():
    ConnectDB()
    sql = 'drop table teams'
    try:
        cursor.execute(sql)
    except:
        pass
    sql = 'drop table nextplace'
    try:
        cursor.execute(sql)
    except:
        pass
    for i in range(1000,1000+N):
        sql = 'drop table site%dscore' % i
        try:
            cursor.execute(sql)
        except:
            pass
        sql = 'drop table site%ddetail' % i
        try:
            cursor.execute(sql)
        except:
            pass
    
    conn.commit()
    ExitDB()

def GetPlace(team):
        ConnectDB()
        sql = "select place from nextplace where route='%s'" % (team['route']+str(team['AC']-1+1000))
        cursor.execute(sql)
        alldata = cursor.fetchall()
        ExitDB()
        try:
            return str(alldata[0][0])
        except:
            return ''
            
def SetPlace(teamid,placeName):
    ConnectDB()
    sql = "update teams set route='%s' where id=%d" % (placeName,teamid)
    cursor.execute(sql)
    conn.commit()
    ExitDB()
    
def GetQues(id):
    ConnectDB()
    sql = 'select * from site%ddetail' % (id)
    cursor.execute(sql)
    alldata = cursor.fetchall()
    ques = []
    ti = 1
    for rec in alldata:
        #id,text,pic,truth,abcd
        tt = ['第'+str(ti)+'题']
        if rec[1] == None:
            pass
            #tt = []
        else:
            tt += rec[1].split('|')
                
        w = {
            'id':ti,
            'text':tt,
            'pic':rec[2],
            'truth':rec[3],
            'choices':[rec[4],rec[5],rec[6],rec[7]]
        }
        
        if w['truth'] ==None:
            w['truth'] = ''
        for i in range(len(w['choices'])):
            if w['choices'][i] == None:
                w['choices'][i] = ''
        
        ques.append(w)
        ti += 1
    ExitDB()
    return ques

def GetTeamScores(id,actimes):
    sc = []
    for i in range(N):
        rec = teamScore[id][i]
        if i >= actimes:
        	rec[2] = '-'
        sc.append(rec)
    return sc

def UpdateTeamScore():
	ConnectDB()
	for i in range(N):
		sql = 'select id,startTime,endTime,useTime,penny from site%dscore' % (i+1000)
		cursor.execute(sql)
		alldata = cursor.fetchall()
		for r in alldata:
			#刷表
			teamScore[r[0]][i]=[r[1],r[2],r[3],r[4]]
	ExitDB()
 
def GetTeamInfo(filt = ''):
    ConnectDB()
    sql = 'select * from teams %s order by AC desc,penny' % filt#desc是逆序
    cursor.execute(sql)
    alldata = cursor.fetchall()
    
    teamsDetail = []
    
    if alldata:
        rank = 1
        count = 0
        lastPe = -1
        #emptyList = []
        #for i in range(N):
        #    emptyList.append([0,0,'-','0'])
        if len(filt) == 0:
        	UpdateTeamScore()
        	#这是不好的做法，但为了方便，现在暂时这样做，批量查找和单个查找的设计与效率
            
        for rec in alldata:
                team = {
                        'id':rec[0],
                        'name':rec[1],
                        'mates':[rec[2],rec[3],rec[4],rec[5]],
                        'state':rec[6],
                        'AC':rec[7],
                        'penny':rec[8],
                        'reward':rec[9],
                        'route':rec[10]
                }
                for i in range(len(team['mates'])):
                    if team['mates'][i] == None:
                        team['mates'][i] = ''
                        
                team['rank'] = '-'
                team['scList'] = GetTeamScores(rec[0],rec[7])
                if team['state'] != 0:
                    if team['penny'] != lastPe:
                        rank += count
                        count = 0
                        lastPe = team['penny']
                    count += 1
                    team['rank'] = rank
                else:
                    pass#team['scList'] = emptyList
                    
                    #pe = 0
                    #for (stTime,endTime,useTime,penny) in team['scList']:
                    #    pe += useTime + penny
                    #team['penny'] = pe
                    
                    
                teamsDetail.append(team)
    ExitDB()
    return teamsDetail
 
def GetTeam(id):
    team = GetTeamInfo('where id = %d' % id)
    try:
        return team[0]
    except:
        return {}
    
#队伍状态含义
'''
state = 0时未开始

'''

def InputCom(id,com):
    try:
        id = int(id)
    except:
        return "请输入队伍编号"
    team = GetTeam(id)
    if len(team) == 0:
        return '您输入的编号无效'
    else:
        return ''

def TeamFinished(ACTimes):
    print ACTimes,N
    return ACTimes >= N
def ChangeScore(team,pos,finished):
    if TeamFinished(team['AC']):
        return
    ConnectDB()
    teamid = team['id']
    alldata = []
    
    sql = 'select * from site%dscore where id=%d' % (pos+1000,teamid)
    cursor.execute(sql)
    alldata = cursor.fetchall()
        
    #id,stTime,enTime,useTime,penny
    #必须存在
    rec = alldata[0]
    
    if finished:
        if pos != 0:
            pass
        else:
            team['penny'] = -team['reward']

    endTime = time.time()
    useTime = (endTime - rec[1]) #按秒计时
        
    try:
        pint = int(rec[4])
    except:
        pint = 0
    
    if finished:
        pint = pint
    else:
        pint = pint + 3 * 60  #罚三分钟
        
    sql = 'update site%dscore set endTime=%d,useTime=%d,penny=%d where id=%d' % (pos+1000,endTime,useTime,pint,teamid)
    cursor.execute(sql)
    
    if finished:
        team['AC'] += 1
        team['state'] = team['state'] + 1
        if not TeamFinished(team['AC']):
            StartCount(team)    #下一个地点记录创建
        team['penny'] += pint + useTime
        

    
    sql = 'update teams set Tstate=%d,penny=%d,AC=%d where id=%d' % (team['state'],team['penny'],team['AC'],teamid)
    cursor.execute(sql)
    conn.commit()
    ExitDB()
    
def StartCount(team):
    ConnectDB()
    sql = 'insert into site%dscore(id,startTime,endTime,useTime,penny) values (%d,%d,%d,%d,%d)' % (team['state']+1000,team['id'],time.time(),0,0,0)
    cursor.execute(sql)
    conn.commit()
    ExitDB()

def DeleteTeamS():
        ConnectDB()
        sql = 'delete from teams'
        cursor.execute(sql)
        for i in range(1000,1000+N):
            sql = 'delete from site%dscore' % (i)
            try:
                cursor.execute(sql)
            except:
                pass
    
        conn.commit()
        ExitDB()    
    
def DeleteTeamScore():
        tid = int(raw_input())
        ConnectDB()
        sql = 'delete from teams where id=%d' % tid
        cursor.execute(sql)
        for i in range(1000,1000+N):
            sql = 'delete from site%dscore where id=%d' % (i,tid)
            try:
                cursor.execute(sql)
            except:
                pass
    
        conn.commit()
        ExitDB()
        
if __name__ == '__main__':
    print 'fix'
    while True:
        print '1:newTeam'
        print '2:newDatabase'
        print '3:deleteTeamScore'
        print '4:newPlace'
        i = raw_input('')
        i = int(i)
        if i == 1:
            #NewTeam('MiraiTeam','Jackie','Hon','Mot','Kun',50)
            #NewTeam('Tr222','Jacrrre','3on','M4t','',50)
            pass
        if i == 2:
            DelDB()
            NewSiteDB()
            NewQues(0,'请输入您的队伍路径编号','','')
            #NewQues(0,'When set up the west ','pic','A','A:2015','B:2025')
            #NewQues(0,'When set up the midle ','pic','B','A:2015','B:2025')
            #NewQues(1,'hahaWhen set up the west ','pic','A','A:2015','B:2025')
        if i == 3:
            DeleteTeamScore()
        if i == 4:
            NewPlace('A1000','信科院大楼')
            NewPlace('A1001','三饭')
