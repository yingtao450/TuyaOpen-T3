#!/bin/bash

APP_BIN_NAME=$1
APP_VERSION=$2
CURR_TARGET_PLATFORM=$3
APP_PATH=../../../$4
USER_CMD=$5

# for test
#TARGET_PLATFORM=bk7258
TARGET_PLATFORM=bk7236

echo APP_BIN_NAME=$APP_BIN_NAME
echo APP_VERSION=$APP_VERSION
echo TARGET_PLATFORM=$TARGET_PLATFORM
echo CURR_TARGET_PLATFORM=$CURR_TARGET_PLATFORM
echo APP_PATH=$APP_PATH
echo USER_CMD=$USER_CMD

PYTHON_CMD="python3"
PIP_CMD="pip3"
enable_python_env() {

    if [ -z $1 ]; then
        echo "Please input virtual environment name."
        exit 1
    fi

    VIRTUAL_NAME=$1
    SCRIPT_DIR=$PWD
    VIRTUAL_ENV=$SCRIPT_DIR/$VIRTUAL_NAME

    echo "SCRIPT_DIR $VIRTUAL_ENV"

    if command -v python3 &>/dev/null; then
        PYTHON_CMD=python3
    elif command -v python &>/dev/null && python --version | grep -q '^Python 3'; then
        PYTHON_CMD=python
    else
        echo "Python 3 is not installed."
        exit 1
    fi

    if [ ! -d "${VIRTUAL_ENV}" ]; then
        echo "Virtual environment not found. Creating one..."
        $PYTHON_CMD -m venv "${VIRTUAL_ENV}" || { echo "Failed to create virtual environment."; exit 1; }
        echo "Virtual environment created at ${VIRTUAL_ENV}"
    else
        echo "Virtual environment already exists."
    fi

    ACTIVATE_SCRIPT=${VIRTUAL_ENV}/bin/activate

    PYTHON_CMD="${VENV_DIR}/bin/python3"

    if [ -f "$ACTIVATE_SCRIPT" ]; then
        echo "Activate python virtual environment."

        source ${ACTIVATE_SCRIPT} || { echo "Failed to activate virtual environment."; exit 1; }

        ${PIP_CMD} install -r "requirements.txt" || { echo "Failed to install required Python packages."; deactivate; exit 1; }
    else
        echo "Activate script not found."
        exit 1
    fi
}

disable_python_env() {
    if [ -z $1 ]; then
        echo "Please input virtual environment name."
        exit 1
    fi

    VIRTUAL_NAME=$1
    SCRIPT_DIR=$PWD
    VIRTUAL_ENV=$SCRIPT_DIR/$VIRTUAL_NAME
    
    echo "SCRIPT_DIR $VIRTUAL_ENV"

    if [ -n "$VIRTUAL_ENV" ]; then
        echo "Deactivate python virtual environment."
        deactivate
    else
        echo "No virtual environment is active."
    fi
}

bash toolchain_get.sh $(pwd)/../toolchain || { echo "Failed to setup toolchain."; exit 1; } 

enable_python_env "tuya_build_env" || { echo "Failed to enable python virtual environment."; exit 1; }

export TUYA_APP_PATH=$APP_PATH
export TUYA_APP_NAME=$APP_BIN_NAME

p=$(pwd);p1=${p%%/vendor*};echo $p1
export TUYA_PROJECT_DIR=$p1

USER_SW_VER=`echo $APP_VERSION | cut -d'-' -f1`

APP_DIR_temp=$(echo $APP_PROJ_PATH)
if [ "x$APP_DIR_temp" != "x" ];then
    last_character=$(echo -n $APP_PROJ_PATH | tail -c 1)
    if [ "x$last_character" = 'x/' ];then
        APP_DIR_temp=${APP_PROJ_PATH%?}
    else
        APP_DIR_temp=$APP_PROJ_PATH
    fi
    APP_DIR=${APP_DIR_temp%/*}
else
    APP_DIR=apps
fi

echo "Start Compile"

APP_PATH=../../$APP_DIR

# Remove TUYA APP OBJs first
if [ -e "${APP_OBJ_PATH}/$APP_BIN_NAME/src" ]; then
for i in `find ${APP_OBJ_PATH}/$APP_BIN_NAME/src -type d`; do
    echo "Deleting $i"
    rm -rf $i/*.o
done
fi

if [ -z $CI_PACKAGE_PATH ]; then
    echo "not is ci build"
else
	make clean -C ./armino/
	rm -rf ./tuya_build
fi

if [ x$USER_CMD = "xclean" ];then
	make clean -C ./armino/
	rm -rf ./tuya_build
	echo "*************************************************************************"
	echo "************************CLEAN SUCCESS************************************"
	echo "*************************************************************************"
	exit 0
fi

export PYTHONPATH=${TUYA_PROJECT_DIR}/vendor/${CURR_TARGET_PLATFORM}/toolchain/site-packages:${PYTHONPATH}

echo "APP_DIR:"$APP_DIR
make ${TARGET_PLATFORM} PROJECT=app PROJECT_DIR=../tuya_app BUILD_DIR=../tuya_build PROJECT_LIBS=app APP_NAME=$APP_BIN_NAME APP_VERSION=$USER_SW_VER APP_DIR=$APP_PATH -j -C ./armino/ APP_VERSION=${APP_VERSION} 
res=$(echo $?)

if [ $res -ne 0 ]; then
    echo "make failed"
    exit -1
fi

echo "Start Combined"

OUTPUT_PATH=""
if [ "x${CI_PACKAGE_PATH}" != "x" ]; then
    echo "ci build"
    OUTPUT_PATH=${CI_PACKAGE_PATH}
else
    echo "local build"
    OUTPUT_PATH=../../../apps/$APP_BIN_NAME/output/$USER_SW_VER
fi

if [ ! -d "$OUTPUT_PATH" ]; then
	mkdir -p $OUTPUT_PATH
fi


if [ -e "./tuya_build/${TARGET_PLATFORM}/all-app.bin" ]; then
    # B_OFFSET=$(grep "_ty_section_start" -r ./tuya_build/${TARGET_PLATFORM}/app.map | awk '{print strtonum($1)}')
	# B_OFFSET_NUM=$(printf "%d" "$((${B_OFFSET}-65536))")
    B_OFFSET=$(python3 ./get_map_section.py ./tuya_build/${TARGET_PLATFORM}/app.map "_ty_section_start")
    if [ $? -ne 0 ]; then
        echo "get map section failed"
        exit -1
    fi
    OFFSET=65536

    B_OFFSET_NUM=$(expr $B_OFFSET - $OFFSET)
    echo $B_OFFSET_NUM

	python3 ./format_up_bin.py ./tuya_build/${TARGET_PLATFORM}/app_pack.bin ./tuya_build/${TARGET_PLATFORM}/app_ug.bin 3B6000  1000 0 1000 18D0 ${B_OFFSET_NUM} -v
    ./diff2ya ./tuya_build/${TARGET_PLATFORM}/app_ug.bin ./tuya_build/${TARGET_PLATFORM}/app_ug.bin ./tuya_build/${TARGET_PLATFORM}/app_ota_ug.bin 0

	cp ./tuya_build/${TARGET_PLATFORM}/app_ota_ug.bin   $OUTPUT_PATH/$APP_BIN_NAME"_UG_"$USER_SW_VER.bin
	cp ./tuya_build/${TARGET_PLATFORM}/app_pack.bin     $OUTPUT_PATH/$APP_BIN_NAME"_UA_"$USER_SW_VER.bin
	cp ./tuya_build/${TARGET_PLATFORM}/all-app.bin      $OUTPUT_PATH/$APP_BIN_NAME"_QIO_"$USER_SW_VER.bin
	cp ./tuya_build/${TARGET_PLATFORM}/app.elf          $OUTPUT_PATH/$APP_BIN_NAME"_"$USER_SW_VER.elf
	cp ./tuya_build/${TARGET_PLATFORM}/app.map          $OUTPUT_PATH/$APP_BIN_NAME"_"$USER_SW_VER.map
	cp ./tuya_build/${TARGET_PLATFORM}/app.nm           $OUTPUT_PATH/$APP_BIN_NAME"_"$USER_SW_VER.nm
	cp ./tuya_build/${TARGET_PLATFORM}/app.txt          $OUTPUT_PATH/$APP_BIN_NAME"_"$USER_SW_VER.txt
	cp ./tuya_build/${TARGET_PLATFORM}/size_map*        $OUTPUT_PATH/
fi

echo "*************************************************************************"
echo "*************************************************************************"
echo "*************************************************************************"
echo "*********************${APP_BIN_NAME}_$APP_VERSION.bin********************"
echo "*************************************************************************"
echo "**********************COMPILE SUCCESS************************************"
echo "*************************************************************************"

disable_python_env "build_env"
