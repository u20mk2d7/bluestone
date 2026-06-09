[ Tier 1 Banks ] 
(JPM, Citi, Goldman Sachs, Deutsche Bank)
       │
       ▼  (Broadcast raw pricing feeds)
[ Tier 1 Venues / Prime Brokers ] 
(LMAX Exchange, Interactive Brokers, Finalto)
       │
       ▼  (Provide API / Retail / Pro Access)
[ Individual / Corporate Pro Accounts ]



https://www.lmax.com/global/demo-login
https://docs.lmax.com/broker-market-data-api/


stunnel.cfg:
# Global settings
socket = l:TCP_NODELAY=1
socket = r:TCP_NODELAY=1
# debug = 7 # check log
fips = yes

[Demo-Trading]
client = yes
accept = 127.0.0.1:40001
connect = fix-order.london-demo.lmax.com:443
sslVersion = TLSv1
options = NO_SSLv2
options = NO_SSLv3

[Demo-MarketData]
client = yes
accept = 127.0.0.1:40003
connect = fix-marketdata.london-demo.lmax.com:443
sslVersion = TLSv1
options = NO_SSLv2
options = NO_SSLv3


test stunnel: lsof -i -P -n | grep stunnel
Test the Connectivity to LMAX: nc -z 127.0.0.1 40001



For your Trading session, use: DataDictionary=path/to/brokerFixTradingGateway-DataDictionary.xml

For your Market Data session, use: DataDictionary=path/to/brokerFixMarketDataGateway-DataDictionary.xml
