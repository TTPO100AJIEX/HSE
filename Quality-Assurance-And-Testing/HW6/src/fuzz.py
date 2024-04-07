from hypothesis import given, strategies as st, settings, Verbosity
from calc import calculate, opn, CalcException
#from correct_calc import calculate, opn, CalcException

# Генерируем более общие арифметические выражения
@given(st.text(min_size=0))
@settings(verbosity=Verbosity.verbose, max_examples=10000)  # Включает подробный вывод
def test_calculate_with_processed_input(input_string):
    processed_input = ""
    try:
        # Сначала преобразуем входное выражение в ОПН
        processed_input = opn(input_string)
    except Exception as e:
        # Ошибки, связанные с обработкой выражения, могут быть проигнорированы
        pass
    ??
    #CalcException, OverflowError и ValueError можно игнорировать и заглушать

if __name__ == "__main__":
    test_calculate_with_processed_input()

