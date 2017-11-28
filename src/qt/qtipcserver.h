#ifndef QTIPCSERVER_H
#define QTIPCSERVER_H

#include <string>

// Define Trustycoin-Qt message queue name for mainnet
#define BITCOINURI_QUEUE_NAME_MAINNET "TrustycoinURI"
// Define Trustycoin-Qt message queue name for testnet
#define BITCOINURI_QUEUE_NAME_TESTNET "TrustycoinURI-testnet"

extern std::string strTrustycoinURIQueueName;

void ipcScanRelay(int argc, char *argv[]);
void ipcInit(int argc, char *argv[]);

#endif // QTIPCSERVER_H
