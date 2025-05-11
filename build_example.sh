#!/bin/sh
# 参数说明：
# $1 - params path: echo_app_top
# $2 - user cmd: build / clean / ...

set -e
cd `dirname $0`

BUILD_PARAM_DIR=$1
BUILD_PARAM_FILE=$BUILD_PARAM_DIR/build_param.config
. $BUILD_PARAM_FILE

APP_BIN_NAME=$CONFIG_PROJECT_NAME
APP_VERSION=$CONFIG_PROJECT_VERSION
HEADER_DIR=$OPEN_HEADER_DIR
LIBS_DIR=$OPEN_LIBS_DIR
LIBS=$PLATFORM_NEED_LIBS
OUTPUT_DIR=$BIN_OUTPUT_DIR
USER_CMD=$2
BOARD_NAME=$PLATFORM_BOARD
TARGET_PLATFORM=bk7236

# echo APP_BIN_NAME=$APP_BIN_NAME
# echo APP_VERSION=$APP_VERSION
# echo USER_CMD=$USER_CMD
# echo LIBS_DIR=$LIBS_DIR
# echo LIBS=$LIBS
# echo OUTPUT_DIR=$OUTPUT_DIR
# echo HEADER_DIR=$HEADER_DIR
# echo TARGET_PLATFORM=$TARGET_PLATFORM
# exit 0

USER_SW_VER=`echo $APP_VERSION | cut -d'-' -f1`

APP_PATH=../../example

PYTHON_CMD="python3"
check_python_install() {

    if command -v python3 >/dev/null; then
        PYTHON_CMD=python3
    elif command -v python >/dev/null && python --version | grep -q '^Python 3'; then
        PYTHON_CMD=python
    else
        echo "Python 3 is not installed. Please run: "
        echo ""
        echo "$ sudo apt-get install python3 -y"
        echo ""
        exit 1
    fi

    python_version=$(${PYTHON_CMD} --version 2>&1 | cut -d' ' -f2 | cut -d. -f1-2)
    formatted_version="python${python_version}-venv"
    echo "Python version: ${python_version}"

    major=$(echo "$python_version" | cut -d. -f1)
    minor=$(echo "$python_version" | cut -d. -f2)

    if [ "$major" -lt 3 ] || { [ "$major" -eq 3 ] && [ "$minor" -lt 8 ]; }; then
        echo "Error: Current Python version (${python_version}) is less than 3.8, please upgrade"
        exit 1
    fi

    if apt list --installed | grep -q "^${formatted_version}/"; then
        echo "${formatted_version} is installed, continuing with the script..."
    else
        echo "python3-venv is not installed. Please run:"
        echo ""
        echo "$ sudo apt-get install python3-venv -y"
        echo ""
        exit 1
    fi 
}

enable_python_env() {
    if [ -z $1 ]; then
        echo "Please input virtual environment name."
        exit 1
    fi

    VIRTUAL_NAME=$1
    SCRIPT_DIR=$PWD
    VIRTUAL_ENV=$SCRIPT_DIR/$VIRTUAL_NAME

    if [ ! -d "${VIRTUAL_ENV}" ]; then
        echo "Virtual environment not found. Creating one..."
        $PYTHON_CMD -m venv "${VIRTUAL_ENV}" || { echo "Failed to create virtual environment."; exit 1; }
        echo "Virtual environment created at ${VIRTUAL_ENV}"
    else
        echo "Virtual environment already exists."
    fi

    ACTIVATE_SCRIPT=${VIRTUAL_ENV}/bin/activate
    PIP_CMD=${VIRTUAL_ENV}/bin/pip3
    if [ -f "$ACTIVATE_SCRIPT" ] && [ -f ${PIP_CMD} ]; then
        echo "Activate python virtual environment."
        . ${ACTIVATE_SCRIPT} || { echo "Failed to activate virtual environment."; exit 1; }
        ${PIP_CMD} install -r "requirements.txt" || { echo "Failed to install required Python packages."; deactivate; exit 1; }
    else
        echo "Activate script not found."
        rm -rf "${VIRTUAL_ENV}"
        $PYTHON_CMD -m venv "${VIRTUAL_ENV}" || { echo "Failed to create virtual environment."; exit 1; }
        . ${ACTIVATE_SCRIPT} || { echo "Failed to activate virtual environment."; exit 1; }
        ${PIP_CMD} install -r "requirements.txt" || { echo "Failed to install required Python packages."; deactivate; exit 1; }

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

check_python_install ||  { echo "Failed to check python environment."; exit 1; }

bash t3_os/toolchain_get.sh $(pwd)/../tools || { echo "Failed to setup toolchain."; exit 1; } 

enable_python_env "tuya_build_env" || { echo "Failed to enable python virtual environment."; exit 1; }

export TUYA_APP_PATH=$APP_PATH
export TUYA_APP_NAME=$APP_BIN_NAME


export TUYA_PROJECT_DIR=$(pwd)
export TUYA_HEADER_DIR=$HEADER_DIR
export TUYA_LIBS_DIR=$LIBS_DIR
export TUYA_LIBS=$LIBS

# Remove TUYA APP OBJs first
if [ -e "${APP_OBJ_PATH}/$APP_BIN_NAME/src" ]; then
for i in `find ${APP_OBJ_PATH}/$APP_BIN_NAME/src -type d`; do
    echo "Deleting $i"
    rm -rf $i/*.o
done
fi

TOP_DIR=$(pwd)
if [ -f ${TOP_DIR}/.app ]; then
    OLD_APP_BIN_NAME=$(cat ${TOP_DIR}/.app)
    echo OLD_APP_BIN_NAME: ${OLD_APP_BIN_NAME}
fi

echo ${APP_BIN_NAME} > ${TOP_DIR}/.app
if [ "$OLD_APP_BIN_NAME" != "$APP_BIN_NAME" ]; then
	make clean -C ./t3_os/armino/
	rm -rf ./t3_os/tuya_build
	echo "AUTO CLEAN SUCCESS"
fi

if [ x$USER_CMD = "xclean" ];then
	make clean -C ./t3_os/armino/
	rm -rf ./t3_os/tuya_build
    rm -rf ./tuya_build_env
	echo "*************************************************************************"
	echo "************************CLEAN SUCCESS************************************"
	echo "*************************************************************************"
	exit 0
fi

cd t3_os

TARGET_PROJECT=tuya_app

echo "Start Compile"

make ${TARGET_PLATFORM} PROJECT=app PROJECT_DIR=../${TARGET_PROJECT} BUILD_DIR=../tuya_build PROJECT_LIBS=app APP_NAME=$APP_BIN_NAME APP_VERSION=$USER_SW_VER APP_DIR=$APP_PATH -j -C ./armino/ APP_VERSION=${APP_VERSION} 

res=$(echo $?)

# delete file whether it exists
rm -f ${TARGET_PROJECT}/.tmp_component_desc
if [ "x${TUYA_LWIP_STACK_USED}" = "xlwip_bk" ]; then
    mv .${TARGET_PLATFORM}_config.save ${TARGET_PROJECT}/config/${TARGET_PLATFORM}/config
fi

if [ $res -ne 0 ]; then
    echo "make failed"
    exit -1
fi

echo "Start Combined"

if [ ! -d "$OUTPUT_DIR" ]; then
	mkdir -p $OUTPUT_DIR
fi

DEBUG_FILE_PATH=${OUTPUT_DIR}/debug
if [ ! -d "$DEBUG_FILE_PATH" ]; then
	mkdir -p $DEBUG_FILE_PATH
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
	
	cp ./tuya_build/${TARGET_PLATFORM}/app_ota_ug.bin   $OUTPUT_DIR/$APP_BIN_NAME"_UG_"$USER_SW_VER.bin
	cp ./tuya_build/${TARGET_PLATFORM}/app_pack.bin     $OUTPUT_DIR/$APP_BIN_NAME"_UA_"$USER_SW_VER.bin
	cp ./tuya_build/${TARGET_PLATFORM}/all-app.bin      $OUTPUT_DIR/$APP_BIN_NAME"_QIO_"$USER_SW_VER.bin

	cp ./tuya_build/${TARGET_PLATFORM}/app.elf          $OUTPUT_DIR/$APP_BIN_NAME"_"$USER_SW_VER.elf
	cp ./tuya_build/${TARGET_PLATFORM}/app.map          $OUTPUT_DIR/$APP_BIN_NAME"_"$USER_SW_VER.map
	cp ./tuya_build/${TARGET_PLATFORM}/app.nm           $OUTPUT_DIR/$APP_BIN_NAME"_"$USER_SW_VER.nm
	cp ./tuya_build/${TARGET_PLATFORM}/app.txt          $OUTPUT_DIR/$APP_BIN_NAME"_"$USER_SW_VER.txt
	cp ./tuya_build/${TARGET_PLATFORM}/size_map*        $OUTPUT_DIR/
fi

echo "*************************************************************************"
echo "*************************************************************************"
echo "*************************************************************************"
echo "*********************${APP_BIN_NAME}_$APP_VERSION.bin********************"
echo "*************************************************************************"
echo "**********************COMPILE SUCCESS************************************"
echo "*************************************************************************"

disable_python_env "build_env"
