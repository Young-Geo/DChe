
dir=$1
rm $dir/app/src/main/jniLibs -rf
mkdir $dir/app/src/main/jniLibs
cd $dir/jni
ndk-build
cp ../libs/armeabi ../app/src/main/jniLibs -R
