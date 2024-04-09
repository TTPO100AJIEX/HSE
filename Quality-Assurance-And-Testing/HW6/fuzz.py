from hypothesis import given, strategies as st, settings, Verbosity
from calc import calculate, opn, CalcException

MAX_EXAMPLES = 100000000

@given(st.text(min_size = 0))
@settings(verbosity = Verbosity.verbose, max_examples = MAX_EXAMPLES)
def test_calculate_with_processed_input(input_string):
    processed_input = ""
    try:
        # Сначала преобразуем входное выражение в ОПН
        processed_input = opn(input_string)
    except Exception as e:
        # Ошибки, связанные с обработкой выражения, могут быть проигнорированы
        pass

    try:
        calculate(processed_input)
    except (CalcException, OverflowError, ValueError) as e:
        pass

if __name__ == "__main__":
    test_calculate_with_processed_input()

