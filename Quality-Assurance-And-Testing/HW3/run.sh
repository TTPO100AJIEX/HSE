pytest --cov=. --cov-report=html:htmlcov --cov-report=term-missing --cov-branch
rm -rf .pytest_cache .benchmarks __pycache__
rm .coverage coverage.xml test-output.xml
pip3 freeze > requirements.txt