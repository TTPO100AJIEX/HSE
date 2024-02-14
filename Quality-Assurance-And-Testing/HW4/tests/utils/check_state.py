from VendingMachine import VendingMachine

def check_state(
    vending_machine: VendingMachine,
    product1_quantity: int = 0,
    product2_quantity: int = 0,
    balance: int = 0,
    mode: VendingMachine.Mode = VendingMachine.Mode.OPERATION,
    sum: int = 0,
    coins1_amount: int = 0,
    coins2_amount: int = 0,
    product1_price: int = 8,
    product2_price: int = 5,
):
    assert vending_machine.getNumberOfProduct1() == product1_quantity
    assert vending_machine.getNumberOfProduct2() == product2_quantity
    assert vending_machine.getCurrentBalance() == balance
    assert vending_machine.getCurrentMode() == mode
    assert vending_machine.getCurrentSum() == sum
    assert vending_machine.getCoins1() == coins1_amount
    assert vending_machine.getCoins2() == coins2_amount
    assert vending_machine.getPrice1() == product1_price
    assert vending_machine.getPrice2() == product2_price