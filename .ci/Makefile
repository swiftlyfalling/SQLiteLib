# Requirements
# ============
#
# Xcode 7+

XCODEBUILD := set -o pipefail && $(shell command -v xcodebuild)

# Xcode Version Information
XCODEVERSION_FULL := $(word 2, $(shell xcodebuild -version))
XCODEVERSION_MAJOR := $(shell xcodebuild -version 2>&1 | grep Xcode | cut -d' ' -f2 | cut -d'.' -f1)
XCODEVERSION_MINOR := $(shell xcodebuild -version 2>&1 | grep Xcode | cut -d' ' -f2 | cut -d'.' -f2)

# The Xcode Version, containing only the "MAJOR.MINOR" (ex. "8.3" for Xcode 8.3, 8.3.1, etc.)
XCODEVERSION := $(XCODEVERSION_MAJOR).$(XCODEVERSION_MINOR)

# Used to determine if xcpretty is available
XCPRETTY_PATH := $(shell command -v xcpretty 2> /dev/null)


# Targets
# =======
#
# make test_build: build SQLiteLib for a specific destination or sdk

default: test_build


# Tests
# =====

# If xcpretty is available, use it for xcodebuild output
XCPRETTY = 
ifdef XCPRETTY_PATH
	XCPRETTY = | xcpretty -c

	# On Travis-CI, use xcpretty-travis-formatter
	ifeq ($(TRAVIS),true)
		XCPRETTY += -f `xcpretty-travis-formatter`
	endif
endif

# Test if SQLiteLib can be built for a specified destination
test_build: clean
ifneq ($(DESTINATION),)

	cd .. && \
	cp SQLiteLib-USER.xcconfig.example SQLiteLib-USER.xcconfig && \
	$(XCODEBUILD) \
	  -project SQLiteLib.xcodeproj \
	  -scheme sqlitelib \
	  -configuration Release \
	  -destination '$(DESTINATION)' \
	  clean build \
	  $(XCPRETTY)

else
ifeq ($(SDK),)
	$(error a DESTINATION or SDK must be specified (see xcodebuild documentation))
endif
	cd .. && \
	cp SQLiteLib-USER.xcconfig.example SQLiteLib-USER.xcconfig && \
	$(XCODEBUILD) \
	  -project SQLiteLib.xcodeproj \
	  -scheme sqlitelib \
	  -configuration Release \
	  -sdk '$(SDK)' \
	  clean build \
	  $(XCPRETTY)
endif

clean:
	cd .. && \
	rm -f sqlite3.h
