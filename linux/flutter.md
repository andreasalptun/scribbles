FLUTTER

In ~/Library:

take install
wget https://storage.googleapis.com/flutter_infra/releases/stable/linux/flutter_linux_1.17.2-stable.tar.xz
wget https://dl.google.com/android/repository/commandlinetools-linux-6514223_latest.zip

cd ..
tar xf install/flutter_linux_1.17.2-stable.tar.xz

take android-sdk
unzip ../install/commandlinetools-linux-6514223_latest.zip
take cmdline-tools
mv ../tools latest

Add flutter and android sdk bin paths to ~/.zshrc, restart terminal session
sdkmanager
flutter doctor

sdkmanager --list
sdkmanager "platforms;android-29" "build-tools;29.0.3"
sdkmanager --update

Set ANDROID_SDK_ROOT and ANDROID_HOME to android-sdk, restart term

sudo apt-get update
sudo apt-get install openjdk-8-jdk

sudo update-alternatives --config java

flutter doctor --android-licenses

