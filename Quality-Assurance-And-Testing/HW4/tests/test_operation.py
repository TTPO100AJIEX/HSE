import pytest

from VendingMachine import VendingMachine
from utils.check_state import check_state

@pytest.fixture
def vmu():
    return VendingMachine()


def test_initial_state(vmu: VendingMachine):
    check_state(vmu)


def test_put_coin_1(vmu: VendingMachine):
    assert vmu.putCoin1() == VendingMachine.Response.OK
    check_state(vmu, balance = 1)
    
def test_put_coin_1_limit(vmu: VendingMachine):
    for _ in range(50):
        assert vmu.putCoin1() == VendingMachine.Response.OK
    assert vmu.putCoin1() == VendingMachine.Response.CANNOT_PERFORM
    check_state(vmu, balance = 50)


def test_put_coin_2(vmu: VendingMachine):
    assert vmu.putCoin2() == VendingMachine.Response.OK
    check_state(vmu, balance = 2)

def test_put_coin_2_limit(vmu: VendingMachine):
    for _ in range(50):
        assert vmu.putCoin2() == VendingMachine.Response.OK
    assert vmu.putCoin2() == VendingMachine.Response.CANNOT_PERFORM
    check_state(vmu, balance = 100)


def test_fill_products(vmu: VendingMachine):
    assert vmu.fillProducts() == VendingMachine.Response.ILLEGAL_OPERATION
    check_state(vmu)
    
def test_fill_coins(vmu: VendingMachine):
    assert vmu.fillCoins(10, 15) == VendingMachine.Response.ILLEGAL_OPERATION
    check_state(vmu)
    
def test_set_prices(vmu: VendingMachine):
    assert vmu.setPrices(10, 15) == VendingMachine.Response.ILLEGAL_OPERATION
    check_state(vmu)
    

def test_enter_admin_ok(vmu: VendingMachine):
    assert vmu.enterAdminMode(117345294655382) == VendingMachine.Response.OK
    check_state(vmu, mode = VendingMachine.Mode.ADMINISTERING)
    
def test_enter_admin_invalid_param(vmu: VendingMachine):
    assert vmu.enterAdminMode(12334535) == VendingMachine.Response.INVALID_PARAM
    check_state(vmu)
    
def test_enter_admin_cannot_perform_ok(vmu: VendingMachine):
    vmu.putCoin1()
    assert vmu.enterAdminMode(117345294655382) == VendingMachine.Response.CANNOT_PERFORM
    check_state(vmu, balance = 1)
    
def test_enter_admin_cannot_perform_invalid(vmu: VendingMachine):
    vmu.putCoin1()
    assert vmu.enterAdminMode(12334535) == VendingMachine.Response.CANNOT_PERFORM
    check_state(vmu, balance = 1)