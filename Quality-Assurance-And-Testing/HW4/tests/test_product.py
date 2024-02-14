import pytest

from VendingMachine import VendingMachine
from utils.check_state import check_state

@pytest.fixture
def vm():
    vm = VendingMachine()
    vm.enterAdminMode(117345294655382)
    vm.fillProducts()
    vm.fillCoins(10, 10)
    vm.exitAdminMode()
    return vm

def test_return_money(vm: VendingMachine):
    pass