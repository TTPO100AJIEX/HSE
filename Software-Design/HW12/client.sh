cd src
javac **/*.java -d ../vm
cd ../
java -classpath vm client/Application $1