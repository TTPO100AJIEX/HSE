import pytest

from VendingMachine import VendingMachine
from utils.check_state import check_state

@pytest.fixture
def vma():
    vm = VendingMachine()
    vm.enterAdminMode(117345294655382)
    return vm


def test_initial_state(vma: VendingMachine):
    check_state(vma, mode = VendingMachine.Mode.ADMINISTERING)

def test_exit_admin(vma: VendingMachine):
    assert vma.exitAdminMode() is None
    check_state(vma, mode = VendingMachine.Mode.OPERATION)


def test_put_coin_1(vma: VendingMachine):
    assert vma.putCoin1() == VendingMachine.Response.ILLEGAL_OPERATION
    check_state(vma, mode = VendingMachine.Mode.ADMINISTERING)

def test_put_coin_2(vma: VendingMachine):
    assert vma.putCoin2() == VendingMachine.Response.ILLEGAL_OPERATION
    check_state(vma, mode = VendingMachine.Mode.ADMINISTERING)


def test_fill_products(vma: VendingMachine):
    assert vma.fillProducts() == VendingMachine.Response.OK
    check_state(vma, mode = VendingMachine.Mode.ADMINISTERING, product1_quantity = 30, product2_quantity = 40)

@pytest.mark.parametrize([ 'c1', 'c2', 'response', 'state' ], [
    [ 5, 7, VendingMachine.Response.OK, { 'coins1_amount': 5, 'coins2_amount': 7, 'sum': 5 + 14 } ],
    [ 50, 7, VendingMachine.Response.OK, { 'coins1_amount': 50, 'coins2_amount': 7, 'sum': 50 + 14 } ],
    [ 5, 50, VendingMachine.Response.OK, { 'coins1_amount': 5, 'coins2_amount': 50, 'sum': 5 + 100 } ],
    [ 50, 50, VendingMachine.Response.OK, { 'coins1_amount': 50, 'coins2_amount': 50, 'sum': 50 + 100 } ],

    [ 0, 5, VendingMachine.Response.INVALID_PARAM, { } ],
    [ 5, 0, VendingMachine.Response.INVALID_PARAM, { } ],
    [ 0, 0, VendingMachine.Response.INVALID_PARAM, { } ],

    [ -5, 5, VendingMachine.Response.INVALID_PARAM, { } ],
    [ 5, -5, VendingMachine.Response.INVALID_PARAM, { } ],
    [ -5, -5, VendingMachine.Response.INVALID_PARAM, { } ],

    [ 5, 75, VendingMachine.Response.INVALID_PARAM, { } ],
    [ 75, 5, VendingMachine.Response.INVALID_PARAM, { } ],
    [ 75, 75, VendingMachine.Response.INVALID_PARAM, { } ]
])
def test_fill_coins(vma: VendingMachine, c1: int, c2: int, response: VendingMachine.Response, state: dict):
    assert vma.fillCoins(c1, c2) == response
    check_state(vma, mode = VendingMachine.Mode.ADMINISTERING, **state)
    
@pytest.mark.parametrize([ 'p1', 'p2', 'response', 'state' ], [
    [ 5, 7, VendingMachine.Response.OK, { 'product1_price': 5, 'product2_price': 7 } ],

    [ -5, 5, VendingMachine.Response.INVALID_PARAM, { } ],
    [ 5, -5, VendingMachine.Response.INVALID_PARAM, { } ],
    [ -5, -5, VendingMachine.Response.INVALID_PARAM, { } ],

    [ 0, 5, VendingMachine.Response.INVALID_PARAM, { } ],
    [ 5, 0, VendingMachine.Response.INVALID_PARAM, { } ],
    [ 0, 0, VendingMachine.Response.INVALID_PARAM, { } ],
])
def test_set_prices(vma: VendingMachine, p1: int, p2: int, response: VendingMachine.Response, state: dict):
    assert vma.setPrices(p1, p2) == response
    check_state(vma, mode = VendingMachine.Mode.ADMINISTERING, **state)
    

def test_return_money(vma: VendingMachine):
    assert vma.returnMoney() == VendingMachine.Response.ILLEGAL_OPERATION
    check_state(vma, mode = VendingMachine.Mode.ADMINISTERING)
    
def test_give_product_1(vma: VendingMachine):
    assert vma.giveProduct1(1) == VendingMachine.Response.ILLEGAL_OPERATION
    check_state(vma, mode = VendingMachine.Mode.ADMINISTERING)
    
def test_give_product_2(vma: VendingMachine):
    assert vma.giveProduct2(1) == VendingMachine.Response.ILLEGAL_OPERATION
    check_state(vma, mode = VendingMachine.Mode.ADMINISTERING)