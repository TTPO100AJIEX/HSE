rm -rf htmlcov
pip3 freeze > requirements.txt
pytest --cov=src --cov-report=html:htmlcov --cov-report=term-missing --cov-branch --slow
rm -f .coverage coverage.xml test-output.xml htmlcov/.gitignore
rm -rf `find . -type d -name __pycache__`
rm -rf .pytest_cache