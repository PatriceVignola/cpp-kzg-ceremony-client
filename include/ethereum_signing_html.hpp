#ifndef ETHEREUM_SIGNING_HTML_HPP
#define ETHEREUM_SIGNING_HTML_HPP

static constexpr auto ethereum_signing_html = R"(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Sign Contribution</title>
    <script src="https://cdn.jsdelivr.net/npm/web3@latest/dist/web3.min.js"></script>
</head>

<body>
    <pre id="banner">${banner}</pre>
    <div id="info"></div>
    <script>
        const ethAddress = '${ethAddress}';
        const callbackPath = '${callbackPath}';
        const jsonDataString = '${jsonData}';

        function setInfoMessage(message) {
            document.getElementById("info").innerHTML = message;
        }

        function setErrorMessage(error) {
            setInfoMessage(`Error occurred while signing: ${error instanceof Error ? error.toString() : JSON.stringify(error)}. You can try refreshing this page and signing again or restart cpp-kzg-ceremony-client with the '--no-signing' option.`);
        }

        async function signContribution() {
            const accounts = await web3.eth.requestAccounts();
            if (!accounts.some(account => account.toLowerCase() == ethAddress.toLowerCase())) {
                setInfoMessage(`Please log in into your wallet with ${ethAddress} instead of ${accounts} and then refresh this page.`)
                return;
            }
            const chainId = await web3.eth.getChainId();
            if (chainId != 1) {
                setInfoMessage(`Please log in into your wallet with mainnet (chainId: 1) instead of chainId: ${chainId} and then refresh this page.`)
                return;
            }
            web3.currentProvider.sendAsync(
                {
                    method: "eth_signTypedData_v4",
                    params: [ethAddress, jsonDataString],
                    from: ethAddress
                },
                async function (error, result) {
                    if (error) {
                        setErrorMessage(error);
                        return;
                    }

                    try {
                        const callbackUrl = callbackPath + "?signature=" + result.result;
                        const response = await fetch(callbackUrl)
                        setInfoMessage("The message was successfully signed! You can close this tab and return to the application.");
                    } catch (error) {
                        setErrorMessage(error);
                    }
                }
            )
        }

        window.addEventListener('load', async () => {
            setInfoMessage(`Please log into your wallet and follow the instructions to sign your contribution with ${ethAddress}.`);

            try {
                if (window.ethereum) {
                    window.web3 = new Web3(ethereum);
                }
                else if (window.web3) {
                    window.web3 = new Web3(web3.currentProvider);
                }
                else if (window.Web3) {
                    window.web3 = new Web3(window.Web3.currentProvider);
                }
                else {
                    setInfoMessage("Failed to find an Ethereum provider in your browser. Restart cpp-kzg-ceremony-client with the `--no-signing` option or install a compatible browser wallet.");
                    return;
                }
                await signContribution();
            }
            catch (error) {
                setErrorMessage(error);
                throw error;
            }
        });
    </script>
</body>

</html>
)";

#endif // ETHEREUM_SIGNING_HTML_HPP