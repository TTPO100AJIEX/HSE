pytest --cov=. --cov-report=term-missing --cov-branch
rm -rf .pytest_cache .benchmarks __pycache__
rm .coverage coverage.xml test-output.xml
pip freeze > requirements.txt