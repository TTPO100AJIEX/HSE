mkdir -p history
rm -rf history/$1
mkdir -p history/$1
cp calc.py history/$1/calc.py
pytest --hypothesis-show-statistics ./fuzz.py > history/$1/output.txt
rm -rf `find . -type d -name __pycache__`
rm -rf .pytest_cache
rm -rf .hypothesis