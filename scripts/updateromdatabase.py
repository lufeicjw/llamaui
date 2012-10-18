#!/usr/bin/python

import sys
import sqlite3
import glob
import subprocess
import re

emulator = { "mame": { "executable": "/usr/games/mame", "romPath": "/usr/local/share/games/mame/roms/" },
			 "stella": { "executable": "/usr/bin/stella", "romPath": "/usr/local/share/games/stella/roms/" } }

if len( sys.argv ) < 2:
	print( "Must pass the database filename on the command line." )
	exit( 1 )
	
fileName = sys.argv[ 1 ]
conn = sqlite3.connect( fileName )
cursor = conn.cursor()

cursor.execute( "create table if not exists roms ( emulator varchar( 32 ), rom varchar( 128 ), description text, last_played timestamp, plays integer default 0, last_play_return_code integer favourite boolean default false, unique (emulator, rom) )" )
cursor.execute( "create table if not exists saved_search ( search_text text )" );

def importMameRoms( cursor ):
	romPath = emulator[ "mame" ][ "romPath" ]
	installedRoms = []
	
	for rom in glob.glob( romPath + "*.zip" ):
		if rom == romPath:
			next
			
		installedRoms.append( rom[ len( romPath ):len( rom ) - 4 ] )

	proc = subprocess.Popen( [ emulator[ "mame" ][ "executable" ], "-listfull" ], stdout=subprocess.PIPE )
	rx = re.compile( '^([a-zA-Z0-9_]*) *"(.*)"\n' )
	
	while True:
		line = proc.stdout.readline()
		
		if line == "":
			break
			
		m = rx.match( line )

		if m == None:
			continue
		
		rom = m.group( 1 )
		description = m.group( 2 )
		
		if rom in installedRoms:
			cursor.execute( "insert or ignore into roms ( emulator, rom, description ) values ( ?, ?, ? )", ( "mame", rom, description ) )

def importStellaRoms( cursor ):
	romPath = emulator[ "stella" ][ "romPath" ]
	romList = []
	
	proc = subprocess.Popen( [ emulator[ "stella" ][ "executable" ], "-listrominfo" ], stdout=subprocess.PIPE )
	rx = re.compile( '^([a-zA-Z0-9_]*) *"(.*)"\n' )
	
	while True:
		line = proc.stdout.readline()
		
		if line == "":
			break
		
		bits = line.split( '|' )
		romList.append( bits[ 1 ] )
		
	for rom in glob.glob( romPath + "*" ):
		if rom == romPath:
			next
			
		name = rom[ len( romPath ):len( rom ) - 4 ]

		if name in romList:
			cursor.execute( "insert or ignore into roms ( emulator, rom, description ) values ( ?, ?, ? )", ( "stella", rom[ len( romPath ): ], name ) )
	
importMameRoms( cursor )
importStellaRoms( cursor )

conn.commit()
cursor.close()

exit( 0 )

