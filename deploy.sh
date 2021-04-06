WEB_DIR="~/Iris/web"
EXE_DIR="~/Iris/executables"
USER="pi"
IP="192.168.1.69"

echo "Creating root directory if missing"
ssh $USER@$IP "mkdir -p ~/Iris"

DIRS="$(ssh $USER@$IP "ls -A ~/Iris")"

if [ -n "$DIRS" ]
then 
echo "Resetting Directories"
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

echo "Finished deployment"