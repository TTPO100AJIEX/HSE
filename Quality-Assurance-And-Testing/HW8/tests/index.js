import assert from 'assert';
import test from 'node:test';
import selenium from 'selenium-webdriver';

function makeSessionId()
{
    const chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const generateCharacter = () => chars[Math.floor(Math.random() * chars.length)];
    return Array(15).fill(null).map(generateCharacter).join('');
}

async function run_test(name, runner)
{
    test(name, async t =>
    {
        const driver = await new selenium.Builder().forBrowser(selenium.Browser.FIREFOX).build();
        await driver.get('http://ruswizard.ddns.net:8091/');
        try { await runner(driver); } finally { await driver.quit(); }
    });
}

run_test('test_authorization_1', async driver =>
{
    if (!(driver instanceof selenium.WebDriver)) throw 'driver is not an instance of WebDriver';

    await driver.findElement(selenium.By.id("sessionId")).clear();
    await driver.findElement(selenium.By.id("sessionId")).sendKeys("0".repeat(20));
    await driver.findElement(selenium.By.id("login-btn")).click();

    const status = driver.findElement(selenium.By.id("status"));
    await driver.wait(selenium.until.elementTextIs(status, "Online"), 1000);
});

run_test('test_authorization_2', async driver =>
{
    if (!(driver instanceof selenium.WebDriver)) throw 'driver is not an instance of WebDriver';
    
    await driver.findElement(selenium.By.id("sessionId")).clear();
    await driver.findElement(selenium.By.id("sessionId")).sendKeys(makeSessionId());
    await driver.findElement(selenium.By.id("login-btn")).click();

    const status = driver.findElement(selenium.By.id("status"));
    await driver.wait(selenium.until.elementTextIs(status, "Online"), 1000);
    const ls = await driver.executeScript("return window.localStorage;");
    assert.notEqual(ls.length, 0);
});

run_test('test_movement_1', async driver =>
{
    if (!(driver instanceof selenium.WebDriver)) throw 'driver is not an instance of WebDriver';
    
    await driver.findElement(selenium.By.id("sessionId")).clear();
    await driver.findElement(selenium.By.id("sessionId")).sendKeys(makeSessionId());
    await driver.findElement(selenium.By.id("login-btn")).click();
    
    const status = driver.findElement(selenium.By.id("status"));
    await driver.wait(selenium.until.elementTextIs(status, "Online"), 1000);

    const place = driver.findElement(selenium.By.id("place"));
    for (let i = 0; i < 30; i++) await driver.findElement(selenium.By.id("arrowLeft")).click();
    await driver.wait(selenium.until.elementTextContains(place, `[-20]`), 3000); // Should not go beyond -20
});

run_test('test_movement_2', async driver =>
{
    if (!(driver instanceof selenium.WebDriver)) throw 'driver is not an instance of WebDriver';
    
    await driver.findElement(selenium.By.id("sessionId")).clear();
    await driver.findElement(selenium.By.id("sessionId")).sendKeys(makeSessionId());
    await driver.findElement(selenium.By.id("login-btn")).click();
    
    const status = driver.findElement(selenium.By.id("status"));
    await driver.wait(selenium.until.elementTextIs(status, "Online"), 1000);

    const place = driver.findElement(selenium.By.id("place"));
    await driver.findElement(selenium.By.id("arrowLeft")).click();
    try
    {
        await driver.wait(selenium.until.elementTextContains(place, `[-1]`), 2000); // Expected to fail
    } catch(e) { return; }
    throw 'Movement took less than 2 seconds';
});

run_test('test_trade_1', async driver =>
{
    if (!(driver instanceof selenium.WebDriver)) throw 'driver is not an instance of WebDriver';
    
    await driver.findElement(selenium.By.id("sessionId")).clear();
    await driver.findElement(selenium.By.id("sessionId")).sendKeys(makeSessionId());
    await driver.findElement(selenium.By.id("login-btn")).click();
    
    const status = driver.findElement(selenium.By.id("status"));
    await driver.wait(selenium.until.elementTextIs(status, "Online"), 1000);
    assert.strictEqual(await driver.findElement(selenium.By.id("act-0-0")).getText(), "Зайти в док");
});

run_test('test_trade_2', async driver =>
{
    if (!(driver instanceof selenium.WebDriver)) throw 'driver is not an instance of WebDriver';
    
    await driver.findElement(selenium.By.id("sessionId")).clear();
    await driver.findElement(selenium.By.id("sessionId")).sendKeys(makeSessionId());
    await driver.findElement(selenium.By.id("login-btn")).click();
    
    const status = driver.findElement(selenium.By.id("status"));
    await driver.wait(selenium.until.elementTextIs(status, "Online"), 1000);
    await driver.findElement(selenium.By.id("act-0-0")).click();

    const content_before = await driver.findElement(selenium.By.id("tradeTable")).getText();
    await driver.findElement(selenium.By.id("item1006sell")).click();
    const content_after = await driver.findElement(selenium.By.id("tradeTable")).getText();
    assert.strictEqual(content_before, content_after);
});