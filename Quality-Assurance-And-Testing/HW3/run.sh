pytest --cov=. --cov-report=html:htmlcov --cov-report=term-missing --cov-branch --slow
rm -rf .pytest_cache .benchmarks __pycache__
rm -f .coverage coverage.xml test-output.xml
pip3 freeze > requirements.txt