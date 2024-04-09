class CalcException(Exception):
    def __init__(self, message):
        super().__init__(message)

def opn(s_in):
    sb_stack, sb_out = [], []
    for c_in in s_in:
        if is_op(c_in):
            while sb_stack and len(sb_stack) > 0:
                c_tmp = sb_stack[-1]
                if is_op(c_tmp) and op_prior(c_in) <= op_prior(c_tmp):
                    sb_out.append(" ")
                    sb_out.append(c_tmp)
                    sb_out.append(" ")
                    del sb_stack[-1]
                else:
                    sb_out.append(" ")
                    break;
            sb_out.append(" ")
            sb_stack.append(c_in)
        elif c_in == '(':
            sb_stack.append(c_in)
        elif c_in == ')':
            c_tmp = sb_stack[-1]
            while "(" != c_tmp:
                if not sb_stack or len(sb_stack) < 1:
                    raise CalcException("Ошибка разбора скобок. Проверьте правильность выражения.")
                sb_out.append(" ")
                sb_out.append(c_tmp)
                del sb_stack[-1]
                c_tmp = sb_stack[-1]
            del sb_stack[-1]
        else:
            sb_out.append(c_in)

    while sb_stack and len(sb_stack) > 0:
        sb_out.append(" ")
        sb_out.append(sb_stack[-1])
        del sb_stack[-1]

    return "".join(sb_out)

def is_op(c):
    return c in "-+*/^!"

def op_prior(op):
    priorities = {'^': 3, '*': 2, '/': 2, '%': 2}
    return priorities.get(op, 1)  # Default to + or -

def calculate(s_in):
    stack = []
    for token in s_in.split():
        try:
            s_tmp = token.strip()
            if len(s_tmp) == 1 and is_op(s_tmp):
                if len(stack) < 2:
                    raise CalcException("Неверное количество данных в стеке для операции " + token)
                b, a = stack.pop(), stack.pop()
                match s_tmp[0]: #get first character
                    case '+':
                        a += b
                    case '-':
                        a -= b
                    case '*':
                        a *= b
                    case '/':
                        if b == 0:
                            return float("inf")
                        a /= b
                    case '%':
                        a %= b
                    case '^':
                        a **= b
                    case _:
                        raise CalcException("Недопустимая операция " + s_tmp)
                stack.append(a)
            else:
                a = float(s_tmp)
                stack.append(a)
        except CalcException:
            raise CalcException("Недопустимый символ в выражении");
    if len(stack) != 1:
        raise CalcException("Количество операторов не соответствует количеству операндов")

    return stack.pop()

if __name__ == '__main__':
    print("Введите выражение для расчета. Поддерживаются цифры, операции +,-,*,/,^,% и приоритеты в виде скобок ( и ):")
    s_in = input()
    s_in = opn(s_in)
    print(calculate(s_in))
