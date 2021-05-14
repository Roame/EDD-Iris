WEB_DIR="~/Iris/web"
EXE_DIR="~/Iris/executables"
CFG_DIR="~/Iris/web/configs"
IMG_DIR="~/Iris/web/images"
USER="pi"
IP="192.168.1.183"

echo "Creating root directory if missing"
ssh $USER@$IP "mkdir -p ~/Iris"

DIRS="$(ssh $USER@$IP "ls -A ~/Iris")"

if [ -n "$DIRS" ]
then 
echo "Resetting directories"
ssh $USER@$IP \
"
rm -r ~/Iris/*
mkdir -p $WEB_DIR &
mkdir -p $EXE_DIR
# "
else 
echo "Creating directories"
ssh $USER@$IP \
"
mkdir -p $WEB_DIR &
mkdir -p $EXE_DIR
# "
fi 

echo "Copying files"
rsync -ae ssh web/ $USER@$IP:$WEB_DIR --delete
rsync -ae ssh code/build/bin/ $USER@$IP:$EXE_DIR --delete

echo "Adjusting permissions"
ssh $USER@$IP "chmod -R 777 $CFG_DIR"
ssh $USER@$IP "chmod -R 777 $IMG_DIR"

echo "Finished deployment"