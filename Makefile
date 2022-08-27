test: test_open_connection

# Test that we can use the custom SQLite library and open a connection.
test_open_connection: build/open_connection
	build/open_connection

build/open_connection: sqlite3.h
	cc test/open_connection.c -Lbuild/Release -lsqlitecustom -I. -o build/open_connection

sqlite3.h: SQLiteLib-USER.xcconfig
	xcodebuild -project SQLiteLib.xcodeproj -target sqlitecustom -sdk macosx

SQLiteLib-USER.xcconfig: SQLiteLib-USER.xcconfig.example
	cp -n SQLiteLib-USER.xcconfig.example SQLiteLib-USER.xcconfig

distclean:
	git clean -dfx .

.PHONY: test test_open_connection distclean
