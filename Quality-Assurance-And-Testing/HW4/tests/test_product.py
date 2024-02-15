import pytest

from VendingMachine import VendingMachine
from utils.check_state import check_state

def create_vm(
    fill_products: bool = False,
    price1: int = 8, price2: int = 5,
    coins1: int = 0, coins2: int = 0,
    put_coins1: int = 0, put_coins2: int = 0
):
    vm = VendingMachine()
    vm.enterAdminMode(117345294655382)
    if fill_products: vm.fillProducts()
    vm.setPrices(price1, price2)
    vm.fillCoins(coins1, coins2)
    vm.exitAdminMode()
    for _ in range(put_coins1): vm.putCoin1()
    for _ in range(put_coins2): vm.putCoin2()
    return vm


@pytest.mark.parametrize([ 'vm', 'response', 'state' ], [
    [
        create_vm(coins1 = 8, coins2 = 12),
        VendingMachine.Response.OK,
        { 'coins1_amount': 8, 'coins2_amount': 12, 'sum': 8 * 1 + 12 * 2 }
    ], # zero balance

    [
        create_vm(coins1 = 8, coins2 = 12, put_coins1 = 30),
        VendingMachine.Response.OK,
        { 'coins1_amount': 8 + 30 - (30 - 24), 'coins2_amount': 0, 'sum': 8 * 1 + 12 * 2 }
    ], # баланс больше суммарной стоимости монет 2 вида

    [
        create_vm(coins1 = 8, coins2 = 12, put_coins1 = 6),
        VendingMachine.Response.OK,
        { 'coins1_amount': 8 + 6, 'coins2_amount': 12 - 3, 'sum': 8 * 1 + 12 * 2 }
    ], # баланс четный
    [
        create_vm(coins1 = 8, coins2 = 12, put_coins1 = 24),
        VendingMachine.Response.OK,
        { 'coins1_amount': 8 + 24, 'coins2_amount': 0, 'sum': 8 * 1 + 12 * 2 }
    ], # баланс равен суммарной стоимости монет 2 вида
    
    [
        create_vm(coins1 = 8, coins2 = 12, put_coins1 = 13),
        VendingMachine.Response.OK,
        { 'coins1_amount': 8 + 13 - 1, 'coins2_amount': 12 - 6, 'sum': 8 * 1 + 12 * 2 }
    ],
    
    [
        create_vm(put_coins1 = 5, put_coins2 = 10),
        VendingMachine.Response.OK,
        { 'coins1_amount': 0, 'coins2_amount': 0, 'sum': 0 }
    ],
    [
        create_vm(put_coins1 = 1, put_coins2 = 10),
        VendingMachine.Response.OK,
        { 'coins1_amount': 0, 'coins2_amount': 0, 'sum': 0 }
    ],
    [
        create_vm(put_coins1 = 0, put_coins2 = 10),
        VendingMachine.Response.OK,
        { 'coins1_amount': 0, 'coins2_amount': 0, 'sum': 0 }
    ]
])
def test_set_prices(vm: VendingMachine, response: VendingMachine.Response, state: dict):
    assert vm.returnMoney() == response
    vm.enterAdminMode(117345294655382)
    check_state(vm, mode = VendingMachine.Mode.ADMINISTERING, **state)


@pytest.mark.parametrize([ 'vm', 'number', 'response', 'state' ], [
    [
        create_vm(fill_products = True, put_coins1 = 50, put_coins2 = 50),
        -1,
        VendingMachine.Response.INVALID_PARAM,
        { 'mode': VendingMachine.Mode.OPERATION, 'balance': 150, 'product1_quantity': 30, 'product2_quantity': 40 }
    ], # < 0 предметов
    [
        create_vm(fill_products = True, put_coins1 = 50, put_coins2 = 50),
        0,
        VendingMachine.Response.INVALID_PARAM,
        { 'mode': VendingMachine.Mode.OPERATION, 'balance': 150, 'product1_quantity': 30, 'product2_quantity': 40 }
    ], # 0 предметов
    [
        create_vm(fill_products = True, put_coins1 = 50, put_coins2 = 50),
        31,
        VendingMachine.Response.INVALID_PARAM,
        { 'mode': VendingMachine.Mode.OPERATION, 'balance': 150, 'product1_quantity': 30, 'product2_quantity': 40 }
    ], # больше максимума предметов 1 вида
    
    [
        create_vm(fill_products = True, put_coins1 = 2, put_coins2 = 1),
        1,
        VendingMachine.Response.INSUFFICIENT_MONEY,
        { 'mode': VendingMachine.Mode.OPERATION, 'balance': 4, 'product1_quantity': 30, 'product2_quantity': 40 }
    ], # при отсутствии на счете требуемой суммы возвращается INSUFFICIENT_MONEY
    
    [
        create_vm(fill_products = True, coins2 = 4, put_coins1 = 8 * 4),
        2,
        VendingMachine.Response.OK,
        { 'mode': VendingMachine.Mode.ADMINISTERING, 'coins1_amount': 16, 'coins2_amount': 0, 'sum': 16, 'product1_quantity': 28, 'product2_quantity': 40 }
    ], # на сдачу не хватает монет 2 вида
    [
        create_vm(fill_products = True, coins1 = 1, coins2 = 10, put_coins1 = 8 * 4),
        2,
        VendingMachine.Response.OK,
        { 'mode': VendingMachine.Mode.ADMINISTERING, 'coins1_amount': 33, 'coins2_amount': 2, 'sum': 37, 'product1_quantity': 28, 'product2_quantity': 40 }
    ], # сдача нацело делится на стоимость монеты 2 вида
    [
        create_vm(fill_products = True, put_coins2 = 8, price1 = 7),
        1,
        VendingMachine.Response.UNSUITABLE_CHANGE,
        { 'mode': VendingMachine.Mode.OPERATION, 'product1_quantity': 30, 'product2_quantity': 40, 'balance': 16, 'product1_price': 7 }
    ], # сдача нечетная, а монет 1 вида нет
    [
        create_vm(fill_products = True, coins1 = 5, coins2 = 6, put_coins1 = 27),
        2,
        VendingMachine.Response.OK,
        { 'mode': VendingMachine.Mode.ADMINISTERING, 'coins1_amount': 31, 'coins2_amount': 1, 'sum': 33, 'product1_quantity': 28, 'product2_quantity': 40 }
    ], # сдача выдается монетами 2 вида когда это возможно, затем — монетами 1 вида
    [
        create_vm(fill_products = True),
        30,
        VendingMachine.Response.INSUFFICIENT_MONEY,
        { 'mode': VendingMachine.Mode.ADMINISTERING, 'product1_quantity': 30, 'product2_quantity': 40 }
    ], # максимум предметов 1 вида
])
def test_give_product_1(vm: VendingMachine, number: int, response: VendingMachine.Response, state: dict):
    assert vm.giveProduct1(number) == response
    vm.enterAdminMode(117345294655382)
    check_state(vm, **state)

def test_give_product_1_insufficient_product():
    vm = create_vm(fill_products = True, put_coins1 = 50, put_coins2 = 50)
    assert vm.giveProduct1(15) == VendingMachine.Response.OK
    assert vm.giveProduct1(20) == VendingMachine.Response.INSUFFICIENT_PRODUCT
    check_state(vm, product1_quantity = 15, product2_quantity = 40)


# Same tests for the second product: why even have separate methods???
@pytest.mark.parametrize([ 'vm', 'number', 'response', 'state' ], [
    [
        create_vm(fill_products = True, put_coins1 = 50, put_coins2 = 50, price2 = 8),
        -1,
        VendingMachine.Response.INVALID_PARAM,
        { 'mode': VendingMachine.Mode.OPERATION, 'balance': 150, 'product1_quantity': 30, 'product2_quantity': 40, 'product2_price': 8 }
    ], # < 0 предметов
    [
        create_vm(fill_products = True, put_coins1 = 50, put_coins2 = 50, price2 = 8),
        0,
        VendingMachine.Response.INVALID_PARAM,
        { 'mode': VendingMachine.Mode.OPERATION, 'balance': 150, 'product1_quantity': 30, 'product2_quantity': 40, 'product2_price': 8  }
    ], # 0 предметов
    [
        create_vm(fill_products = True, put_coins1 = 50, put_coins2 = 50, price2 = 8),
        41,
        VendingMachine.Response.INVALID_PARAM,
        { 'mode': VendingMachine.Mode.OPERATION, 'balance': 150, 'product1_quantity': 30, 'product2_quantity': 40, 'product2_price': 8  }
    ], # больше максимума предметов 2 вида
    [
        create_vm(fill_products = True, put_coins1 = 2, put_coins2 = 1, price2 = 8),
        1,
        VendingMachine.Response.INSUFFICIENT_MONEY,
        { 'mode': VendingMachine.Mode.OPERATION, 'balance': 4, 'product1_quantity': 30, 'product2_quantity': 40, 'product2_price': 8  }
    ], # при отсутствии на счете требуемой суммы возвращается INSUFFICIENT_MONEY

    [
        create_vm(fill_products = True, coins2 = 4, put_coins1 = 8 * 4, price2 = 8),
        2,
        VendingMachine.Response.OK,
        { 'mode': VendingMachine.Mode.ADMINISTERING, 'coins1_amount': 16, 'coins2_amount': 0, 'sum': 16, 'product1_quantity': 30, 'product2_quantity': 38, 'product2_price': 8  }
    ], # на сдачу не хватает монет 2 вида
    [
        create_vm(fill_products = True, coins1 = 1, coins2 = 10, put_coins1 = 8 * 4, price2 = 8),
        2,
        VendingMachine.Response.OK,
        { 'mode': VendingMachine.Mode.ADMINISTERING, 'coins1_amount': 33, 'coins2_amount': 2, 'sum': 37, 'product1_quantity': 30, 'product2_quantity': 38, 'product2_price': 8  }
    ], # сдача нацело делится на стоимость монеты 2 вида
    [
        create_vm(fill_products = True, put_coins2 = 8, price2 = 7),
        1,
        VendingMachine.Response.UNSUITABLE_CHANGE,
        { 'mode': VendingMachine.Mode.OPERATION, 'product1_quantity': 30, 'product2_quantity': 40, 'balance': 16, 'product2_price': 7 }
    ], # сдача нечетная, а монет 2 вида нет
    [
        create_vm(fill_products = True, coins1 = 5, coins2 = 6, put_coins1 = 27, price2 = 8),
        2,
        VendingMachine.Response.OK,
        { 'mode': VendingMachine.Mode.ADMINISTERING, 'coins1_amount': 31, 'coins2_amount': 1, 'sum': 33, 'product1_quantity': 30, 'product2_quantity': 38, 'product2_price': 8  }
    ], # сдача выдается монетами 2 вида когда это возможно, затем — монетами 2 вида
    [
        create_vm(fill_products = True, price2 = 8),
        40,
        VendingMachine.Response.INSUFFICIENT_MONEY,
        { 'mode': VendingMachine.Mode.ADMINISTERING, 'product1_quantity': 30, 'product2_quantity': 40, 'product2_price': 8  }
    ], # максимум предметов 2 вида
])
def test_give_product_2(vm: VendingMachine, number: int, response: VendingMachine.Response, state: dict):
    assert vm.giveProduct2(number) == response
    vm.enterAdminMode(117345294655382)
    check_state(vm, **state)

def test_give_product_2_insufficient_product():
    vm = create_vm(fill_products = True, put_coins1 = 50, put_coins2 = 50)
    assert vm.giveProduct2(25) == VendingMachine.Response.OK
    assert vm.giveProduct2(20) == VendingMachine.Response.INSUFFICIENT_PRODUCT
    check_state(vm, product1_quantity = 30, product2_quantity = 15)