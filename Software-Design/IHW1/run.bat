RMDIR /s /q "vm/reversi"
javac reversi/*.java -d vm
java -classpath vm reversi/Application

PAUSE