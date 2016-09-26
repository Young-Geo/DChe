
dir=$1
mkdir -p $dir/app/src/main/jniLibs/armeabi
cd $dir/jni
ndk-build
cp ../libs/armeabi/* ../app/src/main/jniLibs/armeabi -R
echo "copy end"
