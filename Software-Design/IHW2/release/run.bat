RMDIR /s /q "vm/folder"
javac folder/*.java -d vm
java -classpath vm folder/Application
PAUSE