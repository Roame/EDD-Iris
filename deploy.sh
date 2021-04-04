WEB_DIR="~/Iris/web"
EXE_DIR="~/Iris/executables"

echo "Creating directories"
ssh pi@proto-pi.local \
"mkdir -p $WEB_DIR &
mkdir -p $EXE_DIR"

echo "Copying files"
rsync -ae ssh web/ pi@proto-pi.local:$WEB_DIR --delete
rsync -ae ssh code/build/bin/ pi@proto-pi.local:$EXE_DIR --delete

echo "Finished deployment"