#!/bin/sh

# Copies example .xcconfig file if no user configuration is present

copiedfiles=false
for ((i=0; i < SCRIPT_INPUT_FILE_COUNT ; i++))
    do
    INFILE=SCRIPT_INPUT_FILE_$i
    EXAMPLEFILE=${!INFILE};
    OUTPUTFILE=${EXAMPLEFILE%.example}
    echo "${INFILE}=${EXAMPLEFILE}"
    if [ -f "$EXAMPLEFILE" ]
        then
            if [ ! -f "$OUTPUTFILE" ];
                then
                echo "Expected USER configuration file doesn't exist"
                echo "Copying .example file"
                cp $EXAMPLEFILE $OUTPUTFILE
                touch $OUTPUTFILE
                copiedfiles=true
            else
                echo "USER Configuration file already exists: ${OUTPUTFILE}"
                echo "Skipping .example copy."
            fi
        else
            echo "Missing input .example file: ${EXAMPLEFILE}"
    exit 1
    fi
done

if [ "$copiedfiles" = true ] ; then
    touch "${SRCROOT}/SQLiteLib.xcconfig"
    sleep 1
fi
