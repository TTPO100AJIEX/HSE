set -x
rm -r vm/*
javac library/*.java -d vm
java -classpath vm library/Application