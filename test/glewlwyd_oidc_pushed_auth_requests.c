/* Public domain, no copyright. Use at your own risk. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <gnutls/gnutls.h>
#include <gnutls/crypto.h>
#include <check.h>

#include <orcania.h>
#include <yder.h>
#include <ulfius.h>
#include <rhonabwy.h>

#include "unit-tests.h"

#define SERVER_URI "http://localhost:4593/api"
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "password"
#define USER_USERNAME "user1"
#define USER_PASSWORD "password"
#define PLUGIN_MODULE "oidc"
#define PLUGIN_NAME "oidc_par"
#define PLUGIN_ISS "https://glewlwyd.tld"
#define PLUGIN_JWT_TYPE_RSA "rsa"
#define PLUGIN_JWT_KEY_SIZE "256"
#define PLUGIN_PAR_PREFIX "urn:ietf:params:oauth:request_uri:"
#define PLUGIN_PAR_DURATION 90
#define SCOPE_LIST "g_profile openid"
#define SCOPE_1 "g_profile"
#define SCOPE_2 "openid"
#define CLIENT "client1_id"
#define CLIENT_REDIRECT "../../test-oidc.html?param=client1_cb1"
#define RESPONSE_TYPE "code token"
#define RAR1 "type1"
#define ENRICHED1 "name"
#define ENRICHED2 "email"
#define CLIENT_PUBKEY_ID "client_par"
#define CLIENT_PUBKEY_SECRET "secret_string"
#define CLIENT_PUBKEY_AUTH_TOKEN_MAX_AGE 3600
#define CLIENT_PUBKEY_PARAM "pubkey"
#define CLIENT_PUBKEY_NAME "client with pubkey"
#define CLIENT_PUBKEY_REDIRECT "https://glewlwyd.local/"
#define CLIENT_PUBKEY_REDIRECT_ESCAPED "https%3A%2F%2Fglewlwyd.local%2F"
#define CLIENT_SCOPE "scope1"
#define KID_PUB "pubkey"
#define CLIENT_JWKS_PARAM "jwks"
#define CLIENT_JWKS_URI_PARAM "jwks_uri"
#define PLUGIN_CODE_DURATION 600
#define PLUGIN_REFRESH_TOKEN_DURATION 1209600
#define PLUGIN_ACCESS_TOKEN_DURATION 3600
#define STATE "state1234"
#define NONCE "nonce1234"
#define CLIENT_AUTH_TOKEN_MAX_AGE 3600
#define CODE_CHALLENGE_VALID "V0UN9ToT-UnbxeIx7imQdhFjsAZTmuARpHyuD2ajIIo"
#define CODE_VERIFIER_VALID "XvkLR4XIl4DbkFz3RLEZUBStp8yIjvF8UtfRv0nkK8DqmrBtWvHmEuyBL2enyLF9"
#define CODE_CHALLENGE_METHOD_S256 "S256"

const char pubkey_1_jwk[] = "{\"keys\":[{\"kty\":\"RSA\",\"n\":\"AMWhdXoJpkPtPwABHL_yXUwgcYuwNOVbw70YGmMzhFqiRd6r92-onw-BOAvfnIq-rSMgjidllxOE1fXwlgUIyKJmnHUI3RMDABFmGFRM-Dz6VmQxHgiioLM-Q5yzcj85zIqJvNrw0RL0qhvssQBG5Fta_jLXBUXeGEmciWA0lSfrdlS-zbfxsWqPzAvKyT_0B80m1o8K7ksFtyTPu-cHbCVGx4ciGeZUNrtOnevGQPUOE-tIvsxOPcqC3fPjyI3K4TN5GCCZHEyso1qmRFfsHtenq6EvD1_2DebcODnfnym-iNFyC4YsgqipToNxR3WPIgCu-WrSOk71-93ovs0hd1MhBYw03J4Xupjxy_URCFZm9Pp-9H3j_0hUKmhUWmpsQTpAT7FWvTT-MyyYkZ-9Y33-6KR3E-82kdfXIoEMbGJnfq2Z4Yh_lF3pfD-5FUzOzgnOy0UiTxusWOBbaVhNqmm6xmlHwQjBrax9Bqo7WzQpwXgXgooo6TeVz6pxpUl6V63d5o5XZaxYYilUpZ78qXpHQMwNFr6a2gct-dU8zLF6YaJHIaMp6XT9OD8r-w7SOkq1O7J-UGRqGbUVczYzhApY1Q-B2ZnO18P5KQnG97AbU_Sjk5Rnf6HJ-w-E8NOIwgo5jzloV_5Ck6w-DH_sL5FDca89BGuzwpQEH_h3ma43\",\"e\":\"AQAB\",\"kid\":\"" KID_PUB "\"}]}";
const char privkey_1_jwk[] = "{\"kty\":\"RSA\",\"n\":\"AMWhdXoJpkPtPwABHL_yXUwgcYuwNOVbw70YGmMzhFqiRd6r92-onw-BOAvfnIq-rSMgjidllxOE1fXwlgUIyKJmnHUI3RMDABFmGFRM-Dz6VmQxHgiioLM-Q5yzcj85zIqJvNrw0RL0qhvssQBG5Fta_jLXBUXeGEmciWA0lSfrdlS-zbfxsWqPzAvKyT_0B80m1o8K7ksFtyTPu-cHbCVGx4ciGeZUNrtOnevGQPUOE-tIvsxOPcqC3fPjyI3K4TN5GCCZHEyso1qmRFfsHtenq6EvD1_2DebcODnfnym-iNFyC4YsgqipToNxR3WPIgCu-WrSOk71-93ovs0hd1MhBYw03J4Xupjxy_URCFZm9Pp-9H3j_0hUKmhUWmpsQTpAT7FWvTT-MyyYkZ-9Y33-6KR3E-82kdfXIoEMbGJnfq2Z4Yh_lF3pfD-5FUzOzgnOy0UiTxusWOBbaVhNqmm6xmlHwQjBrax9Bqo7WzQpwXgXgooo6TeVz6pxpUl6V63d5o5XZaxYYilUpZ78qXpHQMwNFr6a2gct-dU8zLF6YaJHIaMp6XT9OD8r-w7SOkq1O7J-UGRqGbUVczYzhApY1Q-B2ZnO18P5KQnG97AbU_Sjk5Rnf6HJ-w-E8NOIwgo5jzloV_5Ck6w-DH_sL5FDca89BGuzwpQEH_h3ma43\",\"e\":\"AQAB\",\"d\":\"AIiu6F7k-ZcVKHNKUaX3a8tQzPb9gTf3xWKsnuNpJ-q_PG-Ko_EXwBqrFiYwG0ZiJcCbrXVV76zSPGCCal9E-e5H5YGUBcI2Wv-tiroTGcSipslYpxr1zwrozz47ZZKQ2QQfyvvpfdAMYvI5Oxmj7h-4yQJEcCMoPcf7eY-ODnKziP2HkSPdBwVaOpcVQyb2EcczS0VXHAPLCiVtftmD6qnFUA4H6b3BFLFq6BG-5gIWIHSjtUH8AwRiijs5mOVoIWTGJYe2HTpyU_BH-hCM_6_LCQrLT2jg9jBqsoBkRuJKIroolAvSEPOxVNnXqMKHoc6zNVFJ4IXn3rBVXlDlCm69xoe67-X2M4o8LXpdnwFtvao3YYKqAqv1kH0JZE9kJyY3odhXa-SRZpvOCoE3YpDr5UTlRkEWZATQjqtGP7JEq_RQwtDwM1NpANIl4cFAJVhUJbndjMeJqBcA4-NEV6bBjWkenw179H6UuWNXNzXklPsgtMnF_PwcBFKutwnFqHAE5g6w9iHQ5yG7_2m4zModfBiGiSy3cdQ2f3MEHRRoBmqooEGU_6Urrn6iyAFxk_sINEnT_7Emygle_QwP5N-BQuFpD_NWojGirWwOwiWYBHRBXP0ub17bNx7w4gha6CxHnXyJ0MZBayOIMrnQGeWC7o5a932LCTQfegdBh5xh\",\"p\":\"APQQSKxv01Oky-jENQwxiZcpI4a5PzLPFFCgEqIjSRamCzrCQ07e97iqhU1b8IvRwxDtX358pFKAq7tmwpN2QQb1T9fqUwCpeQuMwRsZwoaM7ZcTSj2FZ_2djN1ixQfzqQ21VxkMRbrdyExqCSJXnHMcLeiFmu81dVopV2iwDbUQv4jZe_ktPUTH4HKle48Y0v9pu22lD5cknAQGB1gUNfyJ0PbUxZMITrZDz4khhYgxqvJ7GluYRNv2tezV-bb5leXbSLDrRgTKqcl5ZjkgLm9FRNGZZAmlsCHEeB3nvCs2ePQYDuLgEkNtuu39kpLFJO6j70bjnvpaIAcDVpPmEE8\",\"q\":\"AM9L09Grg2uSrNUGfj9pfpsMn0k5kqV0n3WjX9z5ZLkwLNNrs0SJjb93haO2MPNlyYhctCpPKnfHJKZWaLhFDV6xr-ubf7c3DbBJjPhlV8dUkgmHfIqWDPl6pzN0xC61zC4IE15LgW_JEMpq53fRWnIHdufs-105QO8YOo0CVYKYjqut4hVbYRBSTaeVLb1vj_yhaL0qV7orQoTrpr6Bg20nftBBa-8Md_B5l0QyiSfvOjKnXsjULQdQGbtypQZvu2jUasnUVUQHBgeF5W5WFj8qCGGnmehqY6QissipLoRMcGPaV_gJKisgcorF7sSU_QzcBUmPk377LkzZXGNUYZk\",\"qi\":\"AILHVNisODhO4GC8P709DqGdVdufLZf2Bl7AwjWyYTkpEzEfQCHHUnmOoTCn-OEvnn9lWiaCaTijtlUmos0fCfvSQLk9elciIOmlRk8G1EtnnzYQsTmerLoMJBgQ02hhip8GK47Y7mbZIjaPB625Dv8F4RHd9ZiTzXTGcNc6bldWlNNbbqw9DWS1DORPhdQEPU424qcYvHq_eklFCujWukO8ul3FEZYnTcth2ODSFMb0a0SCuDGkGI8BDI-_4n6-4wIlAXtc8Vt9Ko8WxJjCK_v2Ae9x05eknWZj0JxuyoAjPtJApp0pt25omJwZr_lY5i8T0cL6dDF5nZcA9hN__eo\",\"dp\":\"ALL4hfI9BmCdxhFoX-YTJWw9dJnEmf1uMN12pHNVILGFDVMHRUg-5LT8BkhWFSzSoxJ0nsQoLm95f3Uqw6BS5RhvJx-T603e-K5phumSmD0GduuD77rxavJlZ_ioBwfvu5Yb1kS95RxEqi6uywft6wHWNiv-XUDwmJ-HFVvlTgfqwileIjT04argT0yC4PpsH73AEPs0QRx6chXZPeVu3K_Vd_Co0kEhpGavjy5l8H-QvGSXtRpZrJUIcxu7RSTSHQOzK7jgrjWxT5Q4e6eEW8ioqPByZRNV9rSsV9DGMAwYI9YLFk90NLBRdPQ0MBmEi7KbcEkxfVDkafv6jLBj0q0\",\"dq\":\"AJldyYY7dczVxMcKucbinwfJq-N6E_QTt5JKYDdV0F5utQtqiEQx3MyGejooJkk9yn_3zlfrIElj7cqe7XU_qWeg4L3Y2wHLWnZNxF1WZT4VZMJmGg9SeqDtTNz2C9tfJ4P695FxHX99681GkKAGJPtuaFuo6kQLgu4iJ9eBnZA0nIGJ8VXJuKNhsRBGf4PDEW1gYeRqemNDdEBxNHmHypusd9dOP7OpruccnnyXQwBnrtAhIjBFQldBvPgBFvUPH0GsvqE6VicxZxWTy635RRZQW8kcPfNFGxkpjsqE2OSKxTArL6BT733e0L-5NzD75cho1ASblA2DerriqcbXfCk\",\"kid\":\"" KID_PUB "\"}";
const char pubkey_1_pem[] = "-----BEGIN PUBLIC KEY-----\n"\
                            "MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAxaF1egmmQ+0/AAEcv/Jd\n"\
                            "TCBxi7A05VvDvRgaYzOEWqJF3qv3b6ifD4E4C9+cir6tIyCOJ2WXE4TV9fCWBQjI\n"\
                            "omacdQjdEwMAEWYYVEz4PPpWZDEeCKKgsz5DnLNyPznMiom82vDREvSqG+yxAEbk\n"\
                            "W1r+MtcFRd4YSZyJYDSVJ+t2VL7Nt/Gxao/MC8rJP/QHzSbWjwruSwW3JM+75wds\n"\
                            "JUbHhyIZ5lQ2u06d68ZA9Q4T60i+zE49yoLd8+PIjcrhM3kYIJkcTKyjWqZEV+we\n"\
                            "16eroS8PX/YN5tw4Od+fKb6I0XILhiyCqKlOg3FHdY8iAK75atI6TvX73ei+zSF3\n"\
                            "UyEFjDTcnhe6mPHL9REIVmb0+n70feP/SFQqaFRaamxBOkBPsVa9NP4zLJiRn71j\n"\
                            "ff7opHcT7zaR19cigQxsYmd+rZnhiH+UXel8P7kVTM7OCc7LRSJPG6xY4FtpWE2q\n"\
                            "abrGaUfBCMGtrH0GqjtbNCnBeBeCiijpN5XPqnGlSXpXrd3mjldlrFhiKVSlnvyp\n"\
                            "ekdAzA0WvpraBy351TzMsXphokchoynpdP04Pyv7DtI6SrU7sn5QZGoZtRVzNjOE\n"\
                            "CljVD4HZmc7Xw/kpCcb3sBtT9KOTlGd/ocn7D4Tw04jCCjmPOWhX/kKTrD4Mf+wv\n"\
                            "kUNxrz0Ea7PClAQf+HeZrjcCAwEAAQ==\n"\
                            "-----END PUBLIC KEY-----";
const char privkey_1_pem[] = "-----BEGIN RSA PRIVATE KEY-----\n"\
                            "MIIJKwIBAAKCAgEAxaF1egmmQ+0/AAEcv/JdTCBxi7A05VvDvRgaYzOEWqJF3qv3\n"\
                            "b6ifD4E4C9+cir6tIyCOJ2WXE4TV9fCWBQjIomacdQjdEwMAEWYYVEz4PPpWZDEe\n"\
                            "CKKgsz5DnLNyPznMiom82vDREvSqG+yxAEbkW1r+MtcFRd4YSZyJYDSVJ+t2VL7N\n"\
                            "t/Gxao/MC8rJP/QHzSbWjwruSwW3JM+75wdsJUbHhyIZ5lQ2u06d68ZA9Q4T60i+\n"\
                            "zE49yoLd8+PIjcrhM3kYIJkcTKyjWqZEV+we16eroS8PX/YN5tw4Od+fKb6I0XIL\n"\
                            "hiyCqKlOg3FHdY8iAK75atI6TvX73ei+zSF3UyEFjDTcnhe6mPHL9REIVmb0+n70\n"\
                            "feP/SFQqaFRaamxBOkBPsVa9NP4zLJiRn71jff7opHcT7zaR19cigQxsYmd+rZnh\n"\
                            "iH+UXel8P7kVTM7OCc7LRSJPG6xY4FtpWE2qabrGaUfBCMGtrH0GqjtbNCnBeBeC\n"\
                            "iijpN5XPqnGlSXpXrd3mjldlrFhiKVSlnvypekdAzA0WvpraBy351TzMsXphokch\n"\
                            "oynpdP04Pyv7DtI6SrU7sn5QZGoZtRVzNjOECljVD4HZmc7Xw/kpCcb3sBtT9KOT\n"\
                            "lGd/ocn7D4Tw04jCCjmPOWhX/kKTrD4Mf+wvkUNxrz0Ea7PClAQf+HeZrjcCAwEA\n"\
                            "AQKCAgEAiK7oXuT5lxUoc0pRpfdry1DM9v2BN/fFYqye42kn6r88b4qj8RfAGqsW\n"\
                            "JjAbRmIlwJutdVXvrNI8YIJqX0T57kflgZQFwjZa/62KuhMZxKKmyVinGvXPCujP\n"\
                            "PjtlkpDZBB/K++l90Axi8jk7GaPuH7jJAkRwIyg9x/t5j44OcrOI/YeRI90HBVo6\n"\
                            "lxVDJvYRxzNLRVccA8sKJW1+2YPqqcVQDgfpvcEUsWroEb7mAhYgdKO1QfwDBGKK\n"\
                            "OzmY5WghZMYlh7YdOnJT8Ef6EIz/r8sJCstPaOD2MGqygGRG4koiuiiUC9IQ87FU\n"\
                            "2deowoehzrM1UUnghefesFVeUOUKbr3Gh7rv5fYzijwtel2fAW29qjdhgqoCq/WQ\n"\
                            "fQlkT2QnJjeh2Fdr5JFmm84KgTdikOvlROVGQRZkBNCOq0Y/skSr9FDC0PAzU2kA\n"\
                            "0iXhwUAlWFQlud2Mx4moFwDj40RXpsGNaR6fDXv0fpS5Y1c3NeSU+yC0ycX8/BwE\n"\
                            "Uq63CcWocATmDrD2IdDnIbv/abjMyh18GIaJLLdx1DZ/cwQdFGgGaqigQZT/pSuu\n"\
                            "fqLIAXGT+wg0SdP/sSbKCV79DA/k34FC4WkP81aiMaKtbA7CJZgEdEFc/S5vXts3\n"\
                            "HvDiCFroLEedfInQxkFrI4gyudAZ5YLujlr3fYsJNB96B0GHnGECggEBAPQQSKxv\n"\
                            "01Oky+jENQwxiZcpI4a5PzLPFFCgEqIjSRamCzrCQ07e97iqhU1b8IvRwxDtX358\n"\
                            "pFKAq7tmwpN2QQb1T9fqUwCpeQuMwRsZwoaM7ZcTSj2FZ/2djN1ixQfzqQ21VxkM\n"\
                            "RbrdyExqCSJXnHMcLeiFmu81dVopV2iwDbUQv4jZe/ktPUTH4HKle48Y0v9pu22l\n"\
                            "D5cknAQGB1gUNfyJ0PbUxZMITrZDz4khhYgxqvJ7GluYRNv2tezV+bb5leXbSLDr\n"\
                            "RgTKqcl5ZjkgLm9FRNGZZAmlsCHEeB3nvCs2ePQYDuLgEkNtuu39kpLFJO6j70bj\n"\
                            "nvpaIAcDVpPmEE8CggEBAM9L09Grg2uSrNUGfj9pfpsMn0k5kqV0n3WjX9z5ZLkw\n"\
                            "LNNrs0SJjb93haO2MPNlyYhctCpPKnfHJKZWaLhFDV6xr+ubf7c3DbBJjPhlV8dU\n"\
                            "kgmHfIqWDPl6pzN0xC61zC4IE15LgW/JEMpq53fRWnIHdufs+105QO8YOo0CVYKY\n"\
                            "jqut4hVbYRBSTaeVLb1vj/yhaL0qV7orQoTrpr6Bg20nftBBa+8Md/B5l0QyiSfv\n"\
                            "OjKnXsjULQdQGbtypQZvu2jUasnUVUQHBgeF5W5WFj8qCGGnmehqY6QissipLoRM\n"\
                            "cGPaV/gJKisgcorF7sSU/QzcBUmPk377LkzZXGNUYZkCggEBALL4hfI9BmCdxhFo\n"\
                            "X+YTJWw9dJnEmf1uMN12pHNVILGFDVMHRUg+5LT8BkhWFSzSoxJ0nsQoLm95f3Uq\n"\
                            "w6BS5RhvJx+T603e+K5phumSmD0GduuD77rxavJlZ/ioBwfvu5Yb1kS95RxEqi6u\n"\
                            "ywft6wHWNiv+XUDwmJ+HFVvlTgfqwileIjT04argT0yC4PpsH73AEPs0QRx6chXZ\n"\
                            "PeVu3K/Vd/Co0kEhpGavjy5l8H+QvGSXtRpZrJUIcxu7RSTSHQOzK7jgrjWxT5Q4\n"\
                            "e6eEW8ioqPByZRNV9rSsV9DGMAwYI9YLFk90NLBRdPQ0MBmEi7KbcEkxfVDkafv6\n"\
                            "jLBj0q0CggEBAJldyYY7dczVxMcKucbinwfJq+N6E/QTt5JKYDdV0F5utQtqiEQx\n"\
                            "3MyGejooJkk9yn/3zlfrIElj7cqe7XU/qWeg4L3Y2wHLWnZNxF1WZT4VZMJmGg9S\n"\
                            "eqDtTNz2C9tfJ4P695FxHX99681GkKAGJPtuaFuo6kQLgu4iJ9eBnZA0nIGJ8VXJ\n"\
                            "uKNhsRBGf4PDEW1gYeRqemNDdEBxNHmHypusd9dOP7OpruccnnyXQwBnrtAhIjBF\n"\
                            "QldBvPgBFvUPH0GsvqE6VicxZxWTy635RRZQW8kcPfNFGxkpjsqE2OSKxTArL6BT\n"\
                            "733e0L+5NzD75cho1ASblA2DerriqcbXfCkCggEBAILHVNisODhO4GC8P709DqGd\n"\
                            "VdufLZf2Bl7AwjWyYTkpEzEfQCHHUnmOoTCn+OEvnn9lWiaCaTijtlUmos0fCfvS\n"\
                            "QLk9elciIOmlRk8G1EtnnzYQsTmerLoMJBgQ02hhip8GK47Y7mbZIjaPB625Dv8F\n"\
                            "4RHd9ZiTzXTGcNc6bldWlNNbbqw9DWS1DORPhdQEPU424qcYvHq/eklFCujWukO8\n"\
                            "ul3FEZYnTcth2ODSFMb0a0SCuDGkGI8BDI+/4n6+4wIlAXtc8Vt9Ko8WxJjCK/v2\n"\
                            "Ae9x05eknWZj0JxuyoAjPtJApp0pt25omJwZr/lY5i8T0cL6dDF5nZcA9hN//eo=\n"\
                            "-----END RSA PRIVATE KEY-----\n";
const char pubkey_2_jwk[] = "{\"keys\":[{\"kty\":\"RSA\",\"n\":\"AKLIxChfF0q7zzxrkGFM_Qu8Cx76Cd1AEd76RxXSWvmfzfUk12FYeu1LQpzO0VvpmzuwbhRZk9Vgdo5y51s2k83ht4QvwOOIpr9rlNZrTiILeYhYheTfl2bPaKbDPY1rFFtifLhTytaIXZ18VpBQH-30ycdnyWzu34MYzORiKu7NX-72z2gGjUx_IeIZxkSrloPp9eRSo2JhlAw-UkI-XcjK22cf_A2xptF1__60Ly393lz9q9xxRU21p_R3IpmRenLKoZoeIOEbeTrtiXGZa5K20qPIMjjXpWfpLyiicAGRtPuj7uPjIEh3cBntQdo1JnkNRAHpbCFrywKwh-HP7dyWBXQ3vXzH1HKH-gt2rHjGmvB1tVh4Wh_Hq0pz-ZJRdDRoYUVgdZWEglkP4U_coL5UyUPL_9qHBW7GVIvJUOmy_S3OKYh8jDniv9qM5td8JPThxny8SmBLc7OVeqHr4A-3WidK7Uv4dtYi4Lqirea8Hu0c8t26DMVZTRhbsdEUj-a846BkP75LrkoloMsoJkyIXnjlf8AnbOiA0e7Ns64AcUL7FCtYrKeWIqR1aOADrj8TXMRp8S5LdAwOwDmAsIdo54FVpoyoM0mBtBB4oi1K18jBaeMYckwS0kKVmZ-uaPI8AAas1raHTIq7GbGsCBooc17cNNL0USQ51aROxEFF\",\"e\":\"AQAB\",\"kid\":\"" KID_PUB "\"}]}";
const char privkey_2_jwk[] = "{\"kty\":\"RSA\",\"n\":\"AKLIxChfF0q7zzxrkGFM_Qu8Cx76Cd1AEd76RxXSWvmfzfUk12FYeu1LQpzO0VvpmzuwbhRZk9Vgdo5y51s2k83ht4QvwOOIpr9rlNZrTiILeYhYheTfl2bPaKbDPY1rFFtifLhTytaIXZ18VpBQH-30ycdnyWzu34MYzORiKu7NX-72z2gGjUx_IeIZxkSrloPp9eRSo2JhlAw-UkI-XcjK22cf_A2xptF1__60Ly393lz9q9xxRU21p_R3IpmRenLKoZoeIOEbeTrtiXGZa5K20qPIMjjXpWfpLyiicAGRtPuj7uPjIEh3cBntQdo1JnkNRAHpbCFrywKwh-HP7dyWBXQ3vXzH1HKH-gt2rHjGmvB1tVh4Wh_Hq0pz-ZJRdDRoYUVgdZWEglkP4U_coL5UyUPL_9qHBW7GVIvJUOmy_S3OKYh8jDniv9qM5td8JPThxny8SmBLc7OVeqHr4A-3WidK7Uv4dtYi4Lqirea8Hu0c8t26DMVZTRhbsdEUj-a846BkP75LrkoloMsoJkyIXnjlf8AnbOiA0e7Ns64AcUL7FCtYrKeWIqR1aOADrj8TXMRp8S5LdAwOwDmAsIdo54FVpoyoM0mBtBB4oi1K18jBaeMYckwS0kKVmZ-uaPI8AAas1raHTIq7GbGsCBooc17cNNL0USQ51aROxEFF\",\"e\":\"AQAB\",\"d\":\"IHgLKEJPUwjC_To3QjEpB_4p-bPF4-uzpNYm96NNohzN5-fBThln64znbH-UItEltXIrgsObSSREgYVJwFfSg25SPHuJ7diD6gp7VYlxvDittRRzIIO4nzkflqO600pYdSHf3qRYARKSGaeDXKWeuMfqt2tsMd4zluKLe8JY7ejpCbERDZ7A8FErYP3jHi9bhlRUR4Z0MFtmPErx_WSTMEnGGXu3usOEkqMGvLcT8giBIes3LHErcaSaK5jXvenWkq74LNV1mXDxbV-T2qTPYCQ3P5Pe8JeS1nccgO5liOqXPtoj_DCBb_Li2UkJqYyQb_TFa9wzRTdK9u5fLBtdRiDzBIPWNNbhCuD2eaAEkTSW3XXUzmPSlrGC-CMUVemVOWRntPhEFgXiZz36-HMiXefX1gtYQPBVAQI1nPDYppjAGj7u-sVvtQqsKB4AQzc4ULq_ttHiJrw_TGbFSQ0QCPJ2HvnFTeBWixKICvYiRXMo7_kdUBKH7_P_jaoomzWbX39phfWOWC62UN29HjQM3GN1bkMjdolrKo5l8uBdQD_GVAMqdkund6Jw8fpuLmuhFAeOtoMVAsW2Id-CGy_jsi19cbgRHrVHe4_vu3yvvCuQ5MP0Rndivg9JvLM-Lj83JsbWfUG2UEhPycgTPFcwuTKstTciXQSXLKLFetrcsME\",\"p\":\"AM_5kF7BoxK7Tvus6_se2ExfT3lKwE7EzbTAwEhpW1CBi13AsK-bBNZXE-83zF0usZz9DUJ5BRyH4LATxMDCkp0Vza0tdu00778VOgs8wzuStbu6uoc1L7g4Y9ImYwkqLQQuA4IN_RtSdzeWGOA_to7UuMlrHSHQrlAf0npev9ceo35pBUoC0sVAE8nF9Ov2_lkgBRkuNGeQfp_IHyhdjr7r3U4tbja_E-BEFzGUUBvIYJ-1kXKx7ipmpyP47FrOUojjwKyn-xPpu4JYLrvQ-XaiA-SiO5AlBhn_iIXEb3FqBdnWQ0xSoss31jYnVVo-7muR_sBm5UmA0PtVDtrBcSs\",\"q\":\"AMhfwi-RxWW1JgP4oHPBTeOrw9TkFpVahRF5PAeZICuoVxIgPZUaNP1Hhuv6Otb4I7SNRdQioxz6jVGogpk11QV6llsV4skanAgRqWT8ds7qdAsXAhkFCmD6mAaqGs4qVLUSGrMq_KhqBcCdt2pvhy3yuIQmv_iWSJ-XJlsXbWW8DSNa2mpefenKB5dt35zKZp5xzn_UVUoC4HK7c2ZaLvs0KuUE8pgaAdAFogk7emgQfXXK9TPPRd9jullPZTx3UQ104mShX6Hz_vfuTUipPBFefzSm3OKtXM5ejkDQdYmooxnVtR20SujjJ_0D0j66v-G9L8dd-_HcNUgsp57Kf08\",\"qi\":\"bNq-mJjepyGY5mXTlzAcBFY5HqlDx-Ssapo1waswzRhnXKB15zqU-LQfotBBp7pKxWCytwgOmV2toZf_R0Sqinohh52D0mCvTYldTB5tUpKpnhEGZuOZ24BTtrqNAw2X5NRKTflttiVJgfrMCSDIR8NNX4awGB8iUNZLyn4e5Zjfa7cLCwn7ngINpU1TaCY9gbHSsYcVcpX7ro6Vr93ox6cRtapBWElzAPXDF0YKMiKzA4fTqVtbamVHz64k7xgWVwdWXDbdqOWOtI0weeeHMy2Y9Ktz7eXh0rK0fG1c35qJ-V61oXdyfapmI7uGH2R5J-Q2LDJw_3AMGb6oCsPfdQ\",\"dp\":\"I8yQlk78lA_b86R7ZlmT3-mUE4vTeHuV8SQwtQY1qrDx3Wx6vW-QsJiCnO3c5rlP53cDnkqYn6Wf_o8YkhmsBRAovEOUMhanohu0RxTpgkqpr6vfycBU-3_xZs4mxAAXiZ2mCu__foF-dfoHRCqTcRiaykj-1cBHERG5OEkw-oWSnQLU3z2HLF7wSQ4jL67vb0X8uq3iZWVQ9o4LFvaryJ9vE7LsQs43TKZL28Ps2ituvm8Rn02TcocDBEUn4iWbvWZ-1vl_VZkpJrGpMbkyB8KxqtxmJlTJLRZ4WJZMnJgkc6_XG78puJNe8yloHsWwYqHZ2SKdGz7qOikVCoC7yw\",\"dq\":\"atXb1L81b8BBT7a93lo_7FdF5_nhLKsB7kokvqxfYce0_R4hl6FMhYsgnitiOgI-D2OPysbZD3dr6BEf6Q6x0OUGy_QEYlOExCyelBCkTDjnvI38-Vgdq42Rh2QlPK2HUrAfek4-PpGhFY1CIUbr3Yzf4t5CVwnSGP1fXwxDsQ2uN56WfEZ7fi7RE2Vq589nHa3ye2e8PeUAxUu7AOSuzhOHl2qm6oBbXQ3T0nZbEqdQLYEUchZe2_fxgPL7OF0p4zHiD-OW-OP-mzT9EfPh6iTnUCxz84yZwhLaaCZ9tPMsW3b9xaO-mSOcy6PA8t9htbvIgNVUoyVVZ3EfwmOXsw\",\"kid\":\"" KID_PUB "\"}";
const char pubkey_2_pem[] = "-----BEGIN PUBLIC KEY-----\n"\
                            "MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAosjEKF8XSrvPPGuQYUz9\n"\
                            "C7wLHvoJ3UAR3vpHFdJa+Z/N9STXYVh67UtCnM7RW+mbO7BuFFmT1WB2jnLnWzaT\n"\
                            "zeG3hC/A44imv2uU1mtOIgt5iFiF5N+XZs9opsM9jWsUW2J8uFPK1ohdnXxWkFAf\n"\
                            "7fTJx2fJbO7fgxjM5GIq7s1f7vbPaAaNTH8h4hnGRKuWg+n15FKjYmGUDD5SQj5d\n"\
                            "yMrbZx/8DbGm0XX//rQvLf3eXP2r3HFFTbWn9HcimZF6csqhmh4g4Rt5Ou2JcZlr\n"\
                            "krbSo8gyONelZ+kvKKJwAZG0+6Pu4+MgSHdwGe1B2jUmeQ1EAelsIWvLArCH4c/t\n"\
                            "3JYFdDe9fMfUcof6C3aseMaa8HW1WHhaH8erSnP5klF0NGhhRWB1lYSCWQ/hT9yg\n"\
                            "vlTJQ8v/2ocFbsZUi8lQ6bL9Lc4piHyMOeK/2ozm13wk9OHGfLxKYEtzs5V6oevg\n"\
                            "D7daJ0rtS/h21iLguqKt5rwe7Rzy3boMxVlNGFux0RSP5rzjoGQ/vkuuSiWgyygm\n"\
                            "TIheeOV/wCds6IDR7s2zrgBxQvsUK1isp5YipHVo4AOuPxNcxGnxLkt0DA7AOYCw\n"\
                            "h2jngVWmjKgzSYG0EHiiLUrXyMFp4xhyTBLSQpWZn65o8jwABqzWtodMirsZsawI\n"\
                            "GihzXtw00vRRJDnVpE7EQUUCAwEAAQ==\n"\
                            "-----END PUBLIC KEY-----\n";
const char privkey_2_pem[] = "-----BEGIN RSA PRIVATE KEY-----\n"\
                            "MIIJJwIBAAKCAgEAosjEKF8XSrvPPGuQYUz9C7wLHvoJ3UAR3vpHFdJa+Z/N9STX\n"\
                            "YVh67UtCnM7RW+mbO7BuFFmT1WB2jnLnWzaTzeG3hC/A44imv2uU1mtOIgt5iFiF\n"\
                            "5N+XZs9opsM9jWsUW2J8uFPK1ohdnXxWkFAf7fTJx2fJbO7fgxjM5GIq7s1f7vbP\n"\
                            "aAaNTH8h4hnGRKuWg+n15FKjYmGUDD5SQj5dyMrbZx/8DbGm0XX//rQvLf3eXP2r\n"\
                            "3HFFTbWn9HcimZF6csqhmh4g4Rt5Ou2JcZlrkrbSo8gyONelZ+kvKKJwAZG0+6Pu\n"\
                            "4+MgSHdwGe1B2jUmeQ1EAelsIWvLArCH4c/t3JYFdDe9fMfUcof6C3aseMaa8HW1\n"\
                            "WHhaH8erSnP5klF0NGhhRWB1lYSCWQ/hT9ygvlTJQ8v/2ocFbsZUi8lQ6bL9Lc4p\n"\
                            "iHyMOeK/2ozm13wk9OHGfLxKYEtzs5V6oevgD7daJ0rtS/h21iLguqKt5rwe7Rzy\n"\
                            "3boMxVlNGFux0RSP5rzjoGQ/vkuuSiWgyygmTIheeOV/wCds6IDR7s2zrgBxQvsU\n"\
                            "K1isp5YipHVo4AOuPxNcxGnxLkt0DA7AOYCwh2jngVWmjKgzSYG0EHiiLUrXyMFp\n"\
                            "4xhyTBLSQpWZn65o8jwABqzWtodMirsZsawIGihzXtw00vRRJDnVpE7EQUUCAwEA\n"\
                            "AQKCAgAgeAsoQk9TCML9OjdCMSkH/in5s8Xj67Ok1ib3o02iHM3n58FOGWfrjOds\n"\
                            "f5Qi0SW1ciuCw5tJJESBhUnAV9KDblI8e4nt2IPqCntViXG8OK21FHMgg7ifOR+W\n"\
                            "o7rTSlh1Id/epFgBEpIZp4NcpZ64x+q3a2wx3jOW4ot7wljt6OkJsRENnsDwUStg\n"\
                            "/eMeL1uGVFRHhnQwW2Y8SvH9ZJMwScYZe7e6w4SSowa8txPyCIEh6zcscStxpJor\n"\
                            "mNe96daSrvgs1XWZcPFtX5PapM9gJDc/k97wl5LWdxyA7mWI6pc+2iP8MIFv8uLZ\n"\
                            "SQmpjJBv9MVr3DNFN0r27l8sG11GIPMEg9Y01uEK4PZ5oASRNJbdddTOY9KWsYL4\n"\
                            "IxRV6ZU5ZGe0+EQWBeJnPfr4cyJd59fWC1hA8FUBAjWc8NimmMAaPu76xW+1Cqwo\n"\
                            "HgBDNzhQur+20eImvD9MZsVJDRAI8nYe+cVN4FaLEogK9iJFcyjv+R1QEofv8/+N\n"\
                            "qiibNZtff2mF9Y5YLrZQ3b0eNAzcY3VuQyN2iWsqjmXy4F1AP8ZUAyp2S6d3onDx\n"\
                            "+m4ua6EUB462gxUCxbYh34IbL+OyLX1xuBEetUd7j++7fK+8K5Dkw/RGd2K+D0m8\n"\
                            "sz4uPzcmxtZ9QbZQSE/JyBM8VzC5Mqy1NyJdBJcsosV62tywwQKCAQEAz/mQXsGj\n"\
                            "ErtO+6zr+x7YTF9PeUrATsTNtMDASGlbUIGLXcCwr5sE1lcT7zfMXS6xnP0NQnkF\n"\
                            "HIfgsBPEwMKSnRXNrS127TTvvxU6CzzDO5K1u7q6hzUvuDhj0iZjCSotBC4Dgg39\n"\
                            "G1J3N5YY4D+2jtS4yWsdIdCuUB/Sel6/1x6jfmkFSgLSxUATycX06/b+WSAFGS40\n"\
                            "Z5B+n8gfKF2OvuvdTi1uNr8T4EQXMZRQG8hgn7WRcrHuKmanI/jsWs5SiOPArKf7\n"\
                            "E+m7glguu9D5dqID5KI7kCUGGf+IhcRvcWoF2dZDTFKiyzfWNidVWj7ua5H+wGbl\n"\
                            "SYDQ+1UO2sFxKwKCAQEAyF/CL5HFZbUmA/igc8FN46vD1OQWlVqFEXk8B5kgK6hX\n"\
                            "EiA9lRo0/UeG6/o61vgjtI1F1CKjHPqNUaiCmTXVBXqWWxXiyRqcCBGpZPx2zup0\n"\
                            "CxcCGQUKYPqYBqoazipUtRIasyr8qGoFwJ23am+HLfK4hCa/+JZIn5cmWxdtZbwN\n"\
                            "I1raal596coHl23fnMpmnnHOf9RVSgLgcrtzZlou+zQq5QTymBoB0AWiCTt6aBB9\n"\
                            "dcr1M89F32O6WU9lPHdRDXTiZKFfofP+9+5NSKk8EV5/NKbc4q1czl6OQNB1iaij\n"\
                            "GdW1HbRK6OMn/QPSPrq/4b0vx1378dw1SCynnsp/TwKCAQAjzJCWTvyUD9vzpHtm\n"\
                            "WZPf6ZQTi9N4e5XxJDC1BjWqsPHdbHq9b5CwmIKc7dzmuU/ndwOeSpifpZ/+jxiS\n"\
                            "GawFECi8Q5QyFqeiG7RHFOmCSqmvq9/JwFT7f/FmzibEABeJnaYK7/9+gX51+gdE\n"\
                            "KpNxGJrKSP7VwEcREbk4STD6hZKdAtTfPYcsXvBJDiMvru9vRfy6reJlZVD2jgsW\n"\
                            "9qvIn28TsuxCzjdMpkvbw+zaK26+bxGfTZNyhwMERSfiJZu9Zn7W+X9VmSkmsakx\n"\
                            "uTIHwrGq3GYmVMktFnhYlkycmCRzr9cbvym4k17zKWgexbBiodnZIp0bPuo6KRUK\n"\
                            "gLvLAoIBAGrV29S/NW/AQU+2vd5aP+xXRef54SyrAe5KJL6sX2HHtP0eIZehTIWL\n"\
                            "IJ4rYjoCPg9jj8rG2Q93a+gRH+kOsdDlBsv0BGJThMQsnpQQpEw457yN/PlYHauN\n"\
                            "kYdkJTyth1KwH3pOPj6RoRWNQiFG692M3+LeQlcJ0hj9X18MQ7ENrjeelnxGe34u\n"\
                            "0RNlaufPZx2t8ntnvD3lAMVLuwDkrs4Th5dqpuqAW10N09J2WxKnUC2BFHIWXtv3\n"\
                            "8YDy+zhdKeMx4g/jlvjj/ps0/RHz4eok51Asc/OMmcIS2mgmfbTzLFt2/cWjvpkj\n"\
                            "nMujwPLfYbW7yIDVVKMlVWdxH8Jjl7MCggEAbNq+mJjepyGY5mXTlzAcBFY5HqlD\n"\
                            "x+Ssapo1waswzRhnXKB15zqU+LQfotBBp7pKxWCytwgOmV2toZf/R0Sqinohh52D\n"\
                            "0mCvTYldTB5tUpKpnhEGZuOZ24BTtrqNAw2X5NRKTflttiVJgfrMCSDIR8NNX4aw\n"\
                            "GB8iUNZLyn4e5Zjfa7cLCwn7ngINpU1TaCY9gbHSsYcVcpX7ro6Vr93ox6cRtapB\n"\
                            "WElzAPXDF0YKMiKzA4fTqVtbamVHz64k7xgWVwdWXDbdqOWOtI0weeeHMy2Y9Ktz\n"\
                            "7eXh0rK0fG1c35qJ+V61oXdyfapmI7uGH2R5J+Q2LDJw/3AMGb6oCsPfdQ==\n"\
                            "-----END RSA PRIVATE KEY-----\n";

struct _u_request admin_req;
struct _u_request user_req;

START_TEST(test_oidc_par_add_plugin_required_no)
{
  json_t * j_param = json_pack("{sssssss{sssssssssssisisisosososososososososssosososssisosssosososos{s{s[ss]s[ss]s[ss]s[ss]s[ss]}}}}",
                                "module", PLUGIN_MODULE,
                                "name", PLUGIN_NAME,
                                "display_name", PLUGIN_NAME,
                                "parameters",
                                  "iss", PLUGIN_ISS,
                                  "jwt-type", PLUGIN_JWT_TYPE_RSA,
                                  "jwt-key-size", PLUGIN_JWT_KEY_SIZE,
                                  "key", privkey_2_pem,
                                  "cert", pubkey_2_pem,
                                  "access-token-duration", 3600,
                                  "refresh-token-duration", 1209600,
                                  "code-duration", 600,
                                  "refresh-token-rolling", json_true(),
                                  "allow-non-oidc", json_true(),
                                  "auth-type-code-enabled", json_true(),
                                  "auth-type-token-enabled", json_true(),
                                  "auth-type-id-token-enabled", json_true(),
                                  "auth-type-device-enabled", json_true(),
                                  "auth-type-client-enabled", json_true(),
                                  "auth-type-refresh-enabled", json_true(),
                                  "request-parameter-allow", json_true(),
                                  "client-pubkey-parameter", CLIENT_PUBKEY_PARAM,
                                  "request-parameter-allow-encrypted", json_true(),
                                  "oauth-par-allowed", json_true(),
                                  "oauth-par-required", json_false(),
                                  "oauth-par-request_uri-prefix", PLUGIN_PAR_PREFIX,
                                  "oauth-par-duration", PLUGIN_PAR_DURATION,
                                  "oauth-rar-allowed", json_true(),
                                  "rar-types-client-property", "authorization_data_types",
                                  "rar-allow-auth-unsigned", json_true(),
                                  "rar-allow-auth-unencrypted", json_true(),
                                  "pkce-allowed", json_true(),
                                  "pkce-method-plain-allowed", json_false(),
                                  "rar-types",
                                    RAR1,
                                      "scopes",
                                        SCOPE_1,
                                        SCOPE_2,
                                      "locations",
                                        "https://"RAR1"-1.resource.tld",
                                        "https://"RAR1"-2.resource.tld",
                                      "actions",
                                        "action1-"RAR1,
                                        "action2-"RAR1,
                                      "datatypes",
                                        "type1-"RAR1,
                                        "type2-"RAR1,
                                      "enriched",
                                        ENRICHED1,
                                        ENRICHED2);
  ck_assert_int_eq(run_simple_test(&admin_req, "POST", SERVER_URI "/mod/plugin/", NULL, NULL, j_param, NULL, 200, NULL, NULL, NULL), 1);
  json_decref(j_param);
}
END_TEST

START_TEST(test_oidc_par_add_plugin_required_yes)
{
  json_t * j_param = json_pack("{sssssss{sssssssssssisisisosososososososososssosososssisosssosososos{s{s[ss]s[ss]s[ss]s[ss]s[ss]}}}}",
                                "module", PLUGIN_MODULE,
                                "name", PLUGIN_NAME,
                                "display_name", PLUGIN_NAME,
                                "parameters",
                                  "iss", PLUGIN_ISS,
                                  "jwt-type", PLUGIN_JWT_TYPE_RSA,
                                  "jwt-key-size", PLUGIN_JWT_KEY_SIZE,
                                  "key", privkey_2_pem,
                                  "cert", pubkey_2_pem,
                                  "access-token-duration", 3600,
                                  "refresh-token-duration", 1209600,
                                  "code-duration", 600,
                                  "refresh-token-rolling", json_true(),
                                  "allow-non-oidc", json_true(),
                                  "auth-type-code-enabled", json_true(),
                                  "auth-type-token-enabled", json_true(),
                                  "auth-type-id-token-enabled", json_true(),
                                  "auth-type-device-enabled", json_true(),
                                  "auth-type-client-enabled", json_true(),
                                  "auth-type-refresh-enabled", json_true(),
                                  "request-parameter-allow", json_true(),
                                  "client-pubkey-parameter", CLIENT_PUBKEY_PARAM,
                                  "request-parameter-allow-encrypted", json_true(),
                                  "oauth-par-allowed", json_true(),
                                  "oauth-par-required", json_true(),
                                  "oauth-par-request_uri-prefix", PLUGIN_PAR_PREFIX,
                                  "oauth-par-duration", PLUGIN_PAR_DURATION,
                                  "oauth-rar-allowed", json_true(),
                                  "rar-types-client-property", "authorization_data_types",
                                  "rar-allow-auth-unsigned", json_true(),
                                  "rar-allow-auth-unencrypted", json_true(),
                                  "pkce-allowed", json_true(),
                                  "pkce-method-plain-allowed", json_false(),
                                  "rar-types",
                                    RAR1,
                                      "scopes",
                                        SCOPE_1,
                                        SCOPE_2,
                                      "locations",
                                        "https://"RAR1"-1.resource.tld",
                                        "https://"RAR1"-2.resource.tld",
                                      "actions",
                                        "action1-"RAR1,
                                        "action2-"RAR1,
                                      "datatypes",
                                        "type1-"RAR1,
                                        "type2-"RAR1,
                                      "enriched",
                                        ENRICHED1,
                                        ENRICHED2);
  ck_assert_int_eq(run_simple_test(&admin_req, "POST", SERVER_URI "/mod/plugin/", NULL, NULL, j_param, NULL, 200, NULL, NULL, NULL), 1);
  json_decref(j_param);
}
END_TEST

START_TEST(test_oidc_par_delete_plugin)
{
  ck_assert_int_eq(run_simple_test(&admin_req, "DELETE", SERVER_URI "/mod/plugin/" PLUGIN_NAME, NULL, NULL, NULL, NULL, 200, NULL, NULL, NULL), 1);
}
END_TEST

START_TEST(test_oidc_par_add_client_pubkey)
{
  json_t * j_client = json_pack("{ss ss ss so s[s] s[ssss] s[s] ss so s[s] s[ssss]}", "client_id", CLIENT_PUBKEY_ID, "name", CLIENT_PUBKEY_NAME, "client_secret", CLIENT_PUBKEY_SECRET, "confidential", json_true(), "redirect_uri", CLIENT_PUBKEY_REDIRECT, "authorization_type", "code", "token", "id_token", "client_credentials", "scope", CLIENT_SCOPE, CLIENT_PUBKEY_PARAM, pubkey_1_pem, "enabled", json_true(), "authorization_data_types", RAR1, "token_endpoint_auth_method", "client_secret_basic", "client_secret_post", "client_secret_jwt", "private_key_jwt");
  ck_assert_int_eq(run_simple_test(&admin_req, "POST", SERVER_URI "/client/", NULL, NULL, j_client, NULL, 200, NULL, NULL, NULL), 1);
  json_decref(j_client);

  json_t * j_param = json_pack("{ss}", "scope", SCOPE_LIST);
  ck_assert_int_eq(run_simple_test(&user_req, "PUT", SERVER_URI "/auth/grant/" CLIENT_PUBKEY_ID, NULL, NULL, j_param, NULL, 200, NULL, NULL, NULL), 1);
  json_decref(j_param);
}
END_TEST

START_TEST(test_oidc_par_delete_client_pubkey)
{
  json_t * j_param = json_pack("{ss}", "scope", "");
  ck_assert_int_eq(run_simple_test(&user_req, "PUT", SERVER_URI "/auth/grant/" CLIENT_PUBKEY_ID, NULL, NULL, j_param, NULL, 200, NULL, NULL, NULL), 1);
  json_decref(j_param);
  
  ck_assert_int_eq(run_simple_test(&admin_req, "DELETE", SERVER_URI "/client/" CLIENT_PUBKEY_ID, NULL, NULL, NULL, NULL, 200, NULL, NULL, NULL), 1);
}
END_TEST

START_TEST(test_oidc_par_invalid_parameters)
{
  struct _u_request req;
  struct _u_response resp;
  
  ulfius_init_request(&req);
  
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "POST",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/par"),
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(403, resp.status);
  ulfius_clean_response(&resp);
  
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_POST_BODY_PARAMETER, "response_type", RESPONSE_TYPE,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(403, resp.status);
  ulfius_clean_response(&resp);
  
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_POST_BODY_PARAMETER, "client_id", CLIENT,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(403, resp.status);
  ulfius_clean_response(&resp);
  
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_POST_BODY_PARAMETER, "client_id", NULL,
                                U_OPT_POST_BODY_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(403, resp.status);
  ulfius_clean_response(&resp);
  
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_POST_BODY_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_POST_BODY_PARAMETER, "client_id", CLIENT_PUBKEY_ID,
                                U_OPT_POST_BODY_PARAMETER, "redirect_uri", CLIENT_PUBKEY_REDIRECT,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(403, resp.status);
  ulfius_clean_response(&resp);
  
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_POST_BODY_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_POST_BODY_PARAMETER, "response_type", "error",
                                U_OPT_POST_BODY_PARAMETER, "client_id", CLIENT,
                                U_OPT_POST_BODY_PARAMETER, "redirect_uri", CLIENT_REDIRECT,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(403, resp.status);
  ulfius_clean_response(&resp);
  
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_POST_BODY_PARAMETER, "response_type", RESPONSE_TYPE,
                                U_OPT_POST_BODY_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_POST_BODY_PARAMETER, "redirect_uri", CLIENT_PUBKEY_REDIRECT,
                                U_OPT_AUTH_BASIC_USER, CLIENT_PUBKEY_ID,
                                U_OPT_AUTH_BASIC_PASSWORD, "error",
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(403, resp.status);
  ulfius_clean_response(&resp);
  
  ulfius_clean_request(&req);
}
END_TEST

START_TEST(test_oidc_par_client_public)
{
  struct _u_request req;
  struct _u_response resp;
  json_t * j_response;
  
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "POST",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/par"),
                                U_OPT_POST_BODY_PARAMETER, "response_type", RESPONSE_TYPE,
                                U_OPT_POST_BODY_PARAMETER, "client_id", CLIENT,
                                U_OPT_POST_BODY_PARAMETER, "nonce", NONCE,
                                U_OPT_POST_BODY_PARAMETER, "redirect_uri", CLIENT_REDIRECT,
                                U_OPT_POST_BODY_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(201, resp.status);
  ck_assert_ptr_ne(NULL, j_response = ulfius_get_json_body_response(&resp, NULL));
  ck_assert_int_gt(json_string_length(json_object_get(j_response, "request_uri")), o_strlen(PLUGIN_PAR_PREFIX));
  ck_assert_int_eq(0, o_strncmp(json_string_value(json_object_get(j_response, "request_uri")), PLUGIN_PAR_PREFIX, o_strlen(PLUGIN_PAR_PREFIX)));
  ck_assert_int_eq(PLUGIN_PAR_DURATION, json_integer_value(json_object_get(j_response, "expires_in")));
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  json_decref(j_response);
}
END_TEST

START_TEST(test_oidc_par_client_confidential)
{
  struct _u_request req;
  struct _u_response resp;
  json_t * j_response;
  jwt_t * jwt_request = NULL;
  char * request;
  int rnd;
  char jti[12] = {0};
  
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "POST",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/par"),
                                U_OPT_AUTH_BASIC_USER, CLIENT_PUBKEY_ID,
                                U_OPT_AUTH_BASIC_PASSWORD, CLIENT_PUBKEY_SECRET,
                                U_OPT_POST_BODY_PARAMETER, "response_type", RESPONSE_TYPE,
                                U_OPT_POST_BODY_PARAMETER, "redirect_uri", CLIENT_PUBKEY_REDIRECT,
                                U_OPT_POST_BODY_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_POST_BODY_PARAMETER, "state", STATE,
                                U_OPT_POST_BODY_PARAMETER, "nonce", NONCE,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(201, resp.status);
  ck_assert_ptr_ne(NULL, j_response = ulfius_get_json_body_response(&resp, NULL));
  ck_assert_int_gt(json_string_length(json_object_get(j_response, "request_uri")), o_strlen(PLUGIN_PAR_PREFIX));
  ck_assert_int_eq(0, o_strncmp(json_string_value(json_object_get(j_response, "request_uri")), PLUGIN_PAR_PREFIX, o_strlen(PLUGIN_PAR_PREFIX)));
  ck_assert_int_eq(PLUGIN_PAR_DURATION, json_integer_value(json_object_get(j_response, "expires_in")));
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  json_decref(j_response);
  
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "POST",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/par"),
                                U_OPT_POST_BODY_PARAMETER, "response_type", RESPONSE_TYPE,
                                U_OPT_POST_BODY_PARAMETER, "client_id", CLIENT_PUBKEY_ID,
                                U_OPT_AUTH_BASIC_USER, CLIENT_PUBKEY_ID,
                                U_OPT_AUTH_BASIC_PASSWORD, CLIENT_PUBKEY_SECRET,
                                U_OPT_POST_BODY_PARAMETER, "redirect_uri", CLIENT_PUBKEY_REDIRECT,
                                U_OPT_POST_BODY_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_POST_BODY_PARAMETER, "state", STATE,
                                U_OPT_POST_BODY_PARAMETER, "nonce", NONCE,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(201, resp.status);
  ck_assert_ptr_ne(NULL, j_response = ulfius_get_json_body_response(&resp, NULL));
  ck_assert_int_gt(json_string_length(json_object_get(j_response, "request_uri")), o_strlen(PLUGIN_PAR_PREFIX));
  ck_assert_int_eq(0, o_strncmp(json_string_value(json_object_get(j_response, "request_uri")), PLUGIN_PAR_PREFIX, o_strlen(PLUGIN_PAR_PREFIX)));
  ck_assert_int_eq(PLUGIN_PAR_DURATION, json_integer_value(json_object_get(j_response, "expires_in")));
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  json_decref(j_response);
  
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "POST",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/par"),
                                U_OPT_POST_BODY_PARAMETER, "response_type", RESPONSE_TYPE,
                                U_OPT_POST_BODY_PARAMETER, "client_id", CLIENT_PUBKEY_ID,
                                U_OPT_POST_BODY_PARAMETER, "client_secret", CLIENT_PUBKEY_SECRET,
                                U_OPT_POST_BODY_PARAMETER, "redirect_uri", CLIENT_PUBKEY_REDIRECT,
                                U_OPT_POST_BODY_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_POST_BODY_PARAMETER, "state", STATE,
                                U_OPT_POST_BODY_PARAMETER, "nonce", NONCE,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(201, resp.status);
  ck_assert_ptr_ne(NULL, j_response = ulfius_get_json_body_response(&resp, NULL));
  ck_assert_int_gt(json_string_length(json_object_get(j_response, "request_uri")), o_strlen(PLUGIN_PAR_PREFIX));
  ck_assert_int_eq(0, o_strncmp(json_string_value(json_object_get(j_response, "request_uri")), PLUGIN_PAR_PREFIX, o_strlen(PLUGIN_PAR_PREFIX)));
  ck_assert_int_eq(PLUGIN_PAR_DURATION, json_integer_value(json_object_get(j_response, "expires_in")));
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  json_decref(j_response);

  r_jwt_init(&jwt_request);
  ck_assert_ptr_ne(jwt_request, NULL);
  ck_assert_int_eq(r_jwt_set_sign_alg(jwt_request, R_JWA_ALG_RS256), RHN_OK);
  ck_assert_int_eq(r_jwt_add_sign_keys_json_str(jwt_request, privkey_1_jwk, NULL), RHN_OK);
  r_jwt_set_claim_str_value(jwt_request, "aud", CLIENT_PUBKEY_REDIRECT);
  r_jwt_set_claim_str_value(jwt_request, "response_type", RESPONSE_TYPE);
  r_jwt_set_claim_str_value(jwt_request, "client_id", CLIENT_PUBKEY_ID);
  r_jwt_set_claim_str_value(jwt_request, "redirect_uri", CLIENT_PUBKEY_REDIRECT);
  r_jwt_set_claim_str_value(jwt_request, "scope", SCOPE_LIST);
  r_jwt_set_claim_str_value(jwt_request, "state", STATE);
  r_jwt_set_claim_str_value(jwt_request, "nonce", NONCE);
  r_jwt_set_header_str_value(jwt_request, "kid", KID_PUB);
  request = r_jwt_serialize_signed(jwt_request, NULL, 0);
  ck_assert_ptr_ne(request, NULL);
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "POST",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/par"),
                                U_OPT_POST_BODY_PARAMETER, "request", request,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(201, resp.status);
  ck_assert_ptr_ne(NULL, j_response = ulfius_get_json_body_response(&resp, NULL));
  ck_assert_int_gt(json_string_length(json_object_get(j_response, "request_uri")), o_strlen(PLUGIN_PAR_PREFIX));
  ck_assert_int_eq(0, o_strncmp(json_string_value(json_object_get(j_response, "request_uri")), PLUGIN_PAR_PREFIX, o_strlen(PLUGIN_PAR_PREFIX)));
  ck_assert_int_eq(PLUGIN_PAR_DURATION, json_integer_value(json_object_get(j_response, "expires_in")));
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  json_decref(j_response);
  o_free(request);
  r_jwt_free(jwt_request);
  jwt_request = NULL;

  r_jwt_init(&jwt_request);
  ck_assert_ptr_ne(jwt_request, NULL);
  r_jwt_set_sign_alg(jwt_request, R_JWA_ALG_HS256);
  r_jwt_set_enc_alg(jwt_request, R_JWA_ALG_RSA1_5);
  r_jwt_set_enc(jwt_request, R_JWA_ENC_A128CBC);
  ck_assert_int_eq(r_jwt_add_sign_key_symmetric(jwt_request, (unsigned char *)CLIENT_PUBKEY_SECRET, o_strlen(CLIENT_PUBKEY_SECRET)), RHN_OK);
  ck_assert_int_eq(r_jwt_add_enc_keys_pem_der(jwt_request, R_FORMAT_PEM, NULL, 0, (unsigned char *)pubkey_2_pem, o_strlen(pubkey_2_pem)), RHN_OK);
  r_jwt_set_claim_str_value(jwt_request, "aud", CLIENT_PUBKEY_REDIRECT);
  r_jwt_set_claim_str_value(jwt_request, "response_type", RESPONSE_TYPE);
  r_jwt_set_claim_str_value(jwt_request, "client_id", CLIENT_PUBKEY_ID);
  r_jwt_set_claim_str_value(jwt_request, "redirect_uri", CLIENT_PUBKEY_REDIRECT);
  r_jwt_set_claim_str_value(jwt_request, "scope", SCOPE_LIST);
  r_jwt_set_claim_str_value(jwt_request, "state", STATE);
  r_jwt_set_claim_str_value(jwt_request, "nonce", NONCE);
  ck_assert_ptr_ne(request = r_jwt_serialize_nested(jwt_request, R_JWT_TYPE_NESTED_SIGN_THEN_ENCRYPT, NULL, 0, NULL, 0), NULL);
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "POST",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/par"),
                                U_OPT_POST_BODY_PARAMETER, "request", request,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(201, resp.status);
  ck_assert_ptr_ne(NULL, j_response = ulfius_get_json_body_response(&resp, NULL));
  ck_assert_int_gt(json_string_length(json_object_get(j_response, "request_uri")), o_strlen(PLUGIN_PAR_PREFIX));
  ck_assert_int_eq(0, o_strncmp(json_string_value(json_object_get(j_response, "request_uri")), PLUGIN_PAR_PREFIX, o_strlen(PLUGIN_PAR_PREFIX)));
  ck_assert_int_eq(PLUGIN_PAR_DURATION, json_integer_value(json_object_get(j_response, "expires_in")));
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  json_decref(j_response);
  o_free(request);
  r_jwt_free(jwt_request);
  jwt_request = NULL;

  gnutls_rnd(GNUTLS_RND_NONCE, &rnd, sizeof(int));
  snprintf(jti, 11, "jti_%06d", rnd);
  r_jwt_init(&jwt_request);
  ck_assert_ptr_ne(jwt_request, NULL);
  ck_assert_int_eq(r_jwt_set_sign_alg(jwt_request, R_JWA_ALG_RS256), RHN_OK);
  ck_assert_int_eq(r_jwt_add_sign_keys_json_str(jwt_request, privkey_1_jwk, NULL), RHN_OK);
  r_jwt_set_claim_str_value(jwt_request, "iss", CLIENT_PUBKEY_ID);
  r_jwt_set_claim_str_value(jwt_request, "sub", CLIENT_PUBKEY_ID);
  r_jwt_set_claim_str_value(jwt_request, "aud", SERVER_URI "/" PLUGIN_NAME "/par");
  r_jwt_set_claim_str_value(jwt_request, "jti", jti);
  r_jwt_set_claim_int_value(jwt_request, "exp", time(NULL)+(CLIENT_AUTH_TOKEN_MAX_AGE/2));
  r_jwt_set_claim_int_value(jwt_request, "iat", time(NULL));
  r_jwt_set_header_str_value(jwt_request, "kid", KID_PUB);
  request = r_jwt_serialize_signed(jwt_request, NULL, 0);
  ck_assert_ptr_ne(request, NULL);
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "POST",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/par"),
                                U_OPT_POST_BODY_PARAMETER, "client_assertion", request,
                                U_OPT_POST_BODY_PARAMETER, "client_assertion_type", "urn:ietf:params:oauth:client-assertion-type:jwt-bearer",
                                U_OPT_POST_BODY_PARAMETER, "response_type", RESPONSE_TYPE,
                                U_OPT_POST_BODY_PARAMETER, "redirect_uri", CLIENT_PUBKEY_REDIRECT,
                                U_OPT_POST_BODY_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_POST_BODY_PARAMETER, "state", STATE,
                                U_OPT_POST_BODY_PARAMETER, "nonce", NONCE,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(201, resp.status);
  ck_assert_ptr_ne(NULL, j_response = ulfius_get_json_body_response(&resp, NULL));
  ck_assert_int_gt(json_string_length(json_object_get(j_response, "request_uri")), o_strlen(PLUGIN_PAR_PREFIX));
  ck_assert_int_eq(0, o_strncmp(json_string_value(json_object_get(j_response, "request_uri")), PLUGIN_PAR_PREFIX, o_strlen(PLUGIN_PAR_PREFIX)));
  ck_assert_int_eq(PLUGIN_PAR_DURATION, json_integer_value(json_object_get(j_response, "expires_in")));
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  json_decref(j_response);
  o_free(request);
  r_jwt_free(jwt_request);
  jwt_request = NULL;
}
END_TEST

START_TEST(test_oidc_par_token_client_public_invalid_parameters)
{
  struct _u_request req;
  struct _u_response resp;
  json_t * j_response;
  char * request_uri;
  
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "POST",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/par"),
                                U_OPT_POST_BODY_PARAMETER, "response_type", RESPONSE_TYPE,
                                U_OPT_POST_BODY_PARAMETER, "client_id", CLIENT,
                                U_OPT_POST_BODY_PARAMETER, "redirect_uri", CLIENT_REDIRECT,
                                U_OPT_POST_BODY_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(201, resp.status);
  ck_assert_ptr_ne(NULL, j_response = ulfius_get_json_body_response(&resp, NULL));
  ck_assert_int_gt(json_string_length(json_object_get(j_response, "request_uri")), o_strlen(PLUGIN_PAR_PREFIX));
  ck_assert_int_eq(0, o_strncmp(json_string_value(json_object_get(j_response, "request_uri")), PLUGIN_PAR_PREFIX, o_strlen(PLUGIN_PAR_PREFIX)));
  ck_assert_int_eq(PLUGIN_PAR_DURATION, json_integer_value(json_object_get(j_response, "expires_in")));
  request_uri = o_strdup(json_string_value(json_object_get(j_response, "request_uri")));
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "GET",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/auth"),
                                U_OPT_URL_PARAMETER, "client_id", "error",
                                U_OPT_URL_PARAMETER, "request_uri", request_uri,
                                U_OPT_URL_PARAMETER, "g_continue", NULL,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(403, resp.status);
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "GET",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/auth"),
                                U_OPT_URL_PARAMETER, "client_id", CLIENT_PUBKEY_ID,
                                U_OPT_URL_PARAMETER, "request_uri", request_uri,
                                U_OPT_URL_PARAMETER, "g_continue", NULL,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(403, resp.status);
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  
  request_uri[o_strlen(request_uri) - 2] = '\0';
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "GET",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/auth"),
                                U_OPT_URL_PARAMETER, "client_id", CLIENT,
                                U_OPT_URL_PARAMETER, "request_uri", request_uri,
                                U_OPT_URL_PARAMETER, "g_continue", NULL,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(403, resp.status);
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  
  o_free(request_uri);
  json_decref(j_response);
}
END_TEST

START_TEST(test_oidc_par_token_client_public_ok)
{
  struct _u_request req;
  struct _u_response resp;
  json_t * j_response;
  char * code;
  
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "POST",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/par"),
                                U_OPT_POST_BODY_PARAMETER, "response_type", RESPONSE_TYPE,
                                U_OPT_POST_BODY_PARAMETER, "client_id", CLIENT,
                                U_OPT_POST_BODY_PARAMETER, "nonce", NONCE,
                                U_OPT_POST_BODY_PARAMETER, "redirect_uri", CLIENT_REDIRECT,
                                U_OPT_POST_BODY_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_POST_BODY_PARAMETER, "code_challenge", CODE_CHALLENGE_VALID,
                                U_OPT_POST_BODY_PARAMETER, "code_challenge_method", CODE_CHALLENGE_METHOD_S256,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(201, resp.status);
  ck_assert_ptr_ne(NULL, j_response = ulfius_get_json_body_response(&resp, NULL));
  ck_assert_int_gt(json_string_length(json_object_get(j_response, "request_uri")), o_strlen(PLUGIN_PAR_PREFIX));
  ck_assert_int_eq(0, o_strncmp(json_string_value(json_object_get(j_response, "request_uri")), PLUGIN_PAR_PREFIX, o_strlen(PLUGIN_PAR_PREFIX)));
  ck_assert_int_eq(PLUGIN_PAR_DURATION, json_integer_value(json_object_get(j_response, "expires_in")));
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "GET",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/auth"),
                                U_OPT_URL_PARAMETER, "client_id", CLIENT,
                                U_OPT_URL_PARAMETER, "nonce", NONCE,
                                U_OPT_URL_PARAMETER, "request_uri", json_string_value(json_object_get(j_response, "request_uri")),
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(302, resp.status);
  ck_assert_ptr_ne(NULL, o_strstr(u_map_get(resp.map_header, "Location"), "login.html"));
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  
  ulfius_init_request(&req);
  ulfius_copy_request(&req, &user_req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "GET",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/auth"),
                                U_OPT_URL_PARAMETER, "client_id", CLIENT,
                                U_OPT_URL_PARAMETER, "nonce", NONCE,
                                U_OPT_URL_PARAMETER, "request_uri", json_string_value(json_object_get(j_response, "request_uri")),
                                U_OPT_URL_PARAMETER, "g_continue", NULL,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(302, resp.status);
  ck_assert_ptr_ne(o_strstr(u_map_get(resp.map_header, "Location"), "access_token="), NULL);
  ck_assert_ptr_ne(o_strstr(u_map_get(resp.map_header, "Location"), "code="), NULL);
  code = o_strdup(strstr(u_map_get(resp.map_header, "Location"), "code=")+strlen("code="));
  if (strchr(code, '&') != NULL) {
    *strchr(code, '&') = '\0';
  }
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  
  ulfius_init_request(&req);
  ulfius_copy_request(&req, &user_req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "POST",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/token"),
                                U_OPT_POST_BODY_PARAMETER, "client_id", CLIENT,
                                U_OPT_POST_BODY_PARAMETER, "redirect_uri", CLIENT_REDIRECT,
                                U_OPT_POST_BODY_PARAMETER, "code_verifier", CODE_VERIFIER_VALID,
                                U_OPT_POST_BODY_PARAMETER, "code", code,
                                U_OPT_POST_BODY_PARAMETER, "grant_type", "authorization_code",
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(200, resp.status);
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  
  o_free(code);
  json_decref(j_response);
}
END_TEST

START_TEST(test_oidc_auth_client_public_ok)
{
  struct _u_request req;
  struct _u_response resp;
  
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "GET",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/auth"),
                                U_OPT_URL_PARAMETER, "client_id", CLIENT,
                                U_OPT_URL_PARAMETER, "nonce", NONCE,
                                U_OPT_URL_PARAMETER, "redirect_uri", CLIENT_REDIRECT,
                                U_OPT_URL_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_URL_PARAMETER, "response_type", RESPONSE_TYPE,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(302, resp.status);
  ck_assert_ptr_ne(NULL, o_strstr(u_map_get(resp.map_header, "Location"), "login.html"));
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  
  ulfius_init_request(&req);
  ulfius_copy_request(&req, &user_req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "GET",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/auth"),
                                U_OPT_URL_PARAMETER, "client_id", CLIENT,
                                U_OPT_URL_PARAMETER, "nonce", NONCE,
                                U_OPT_URL_PARAMETER, "redirect_uri", CLIENT_REDIRECT,
                                U_OPT_URL_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_URL_PARAMETER, "response_type", RESPONSE_TYPE,
                                U_OPT_URL_PARAMETER, "g_continue", NULL,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(302, resp.status);
  ck_assert_ptr_ne(o_strstr(u_map_get(resp.map_header, "Location"), "access_token="), NULL);
  ck_assert_ptr_ne(o_strstr(u_map_get(resp.map_header, "Location"), "code="), NULL);
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  
}
END_TEST

START_TEST(test_oidc_auth_client_public_error)
{
  struct _u_request req;
  struct _u_response resp;
  
  ulfius_init_request(&req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "GET",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/auth"),
                                U_OPT_URL_PARAMETER, "client_id", CLIENT,
                                U_OPT_URL_PARAMETER, "nonce", NONCE,
                                U_OPT_URL_PARAMETER, "redirect_uri", CLIENT_REDIRECT,
                                U_OPT_URL_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_URL_PARAMETER, "response_type", RESPONSE_TYPE,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(403, resp.status);
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  
  ulfius_init_request(&req);
  ulfius_copy_request(&req, &user_req);
  ulfius_init_response(&resp);
  ulfius_set_request_properties(&req, 
                                U_OPT_HTTP_VERB, "GET",
                                U_OPT_HTTP_URL, (SERVER_URI "/" PLUGIN_NAME "/auth"),
                                U_OPT_URL_PARAMETER, "client_id", CLIENT,
                                U_OPT_URL_PARAMETER, "nonce", NONCE,
                                U_OPT_URL_PARAMETER, "redirect_uri", CLIENT_REDIRECT,
                                U_OPT_URL_PARAMETER, "scope", SCOPE_LIST,
                                U_OPT_URL_PARAMETER, "response_type", RESPONSE_TYPE,
                                U_OPT_URL_PARAMETER, "g_continue", NULL,
                                U_OPT_NONE);
  ck_assert_int_eq(U_OK, ulfius_send_http_request(&req, &resp));
  ck_assert_int_eq(403, resp.status);
  ulfius_clean_response(&resp);
  ulfius_clean_request(&req);
  
}
END_TEST

static Suite *glewlwyd_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("Glewlwyd oidc pushed authentication requests");
  tc_core = tcase_create("test_oidc_pushed_authentication_requests");
  tcase_add_test(tc_core, test_oidc_par_add_plugin_required_no);
  tcase_add_test(tc_core, test_oidc_par_add_client_pubkey);
  tcase_add_test(tc_core, test_oidc_par_invalid_parameters);
  tcase_add_test(tc_core, test_oidc_par_client_public);
  tcase_add_test(tc_core, test_oidc_par_client_confidential);
  tcase_add_test(tc_core, test_oidc_par_token_client_public_invalid_parameters);
  tcase_add_test(tc_core, test_oidc_par_token_client_public_ok);
  tcase_add_test(tc_core, test_oidc_auth_client_public_ok);
  tcase_add_test(tc_core, test_oidc_par_delete_plugin);
  tcase_add_test(tc_core, test_oidc_par_add_plugin_required_yes);
  tcase_add_test(tc_core, test_oidc_par_invalid_parameters);
  tcase_add_test(tc_core, test_oidc_par_client_public);
  tcase_add_test(tc_core, test_oidc_par_client_confidential);
  tcase_add_test(tc_core, test_oidc_par_token_client_public_invalid_parameters);
  tcase_add_test(tc_core, test_oidc_par_token_client_public_ok);
  tcase_add_test(tc_core, test_oidc_auth_client_public_error);
  tcase_add_test(tc_core, test_oidc_par_delete_client_pubkey);
  tcase_add_test(tc_core, test_oidc_par_delete_plugin);
  tcase_set_timeout(tc_core, 30);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(int argc, char *argv[])
{
  int number_failed = 0;
  Suite *s;
  SRunner *sr;
  struct _u_request auth_req, scope_req;
  struct _u_response auth_resp, scope_resp;
  int res, do_test = 0;
  char * cookie;
  json_t * j_body;

  y_init_logs("Glewlwyd test", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "Starting Glewlwyd test");

  ulfius_init_request(&admin_req);
  ulfius_init_request(&user_req);

  // Getting a valid session id for authenticated http requests
  ulfius_init_request(&auth_req);
  ulfius_init_request(&scope_req);
  ulfius_init_response(&auth_resp);
  ulfius_init_response(&scope_resp);
  auth_req.http_verb = strdup("POST");
  auth_req.http_url = msprintf("%s/auth/", SERVER_URI);
  j_body = json_pack("{ssss}", "username", ADMIN_USERNAME, "password", ADMIN_PASSWORD);
  ulfius_set_json_body_request(&auth_req, j_body);
  json_decref(j_body);
  res = ulfius_send_http_request(&auth_req, &auth_resp);
  if (res == U_OK && auth_resp.status == 200) {
    if (auth_resp.nb_cookies) {
      y_log_message(Y_LOG_LEVEL_DEBUG, "Admin %s authenticated", ADMIN_USERNAME);
      cookie = msprintf("%s=%s", auth_resp.map_cookie[0].key, auth_resp.map_cookie[0].value);
      u_map_put(admin_req.map_header, "Cookie", cookie);
      o_free(cookie);
      do_test = 1;
    }
  } else {
    y_log_message(Y_LOG_LEVEL_ERROR, "Error authentication admin");
  }
  ulfius_clean_response(&auth_resp);
  ulfius_clean_request(&auth_req);

  if (do_test) {
    // Getting a valid session id for authenticated http requests
    ulfius_init_request(&auth_req);
    ulfius_init_response(&auth_resp);
    auth_req.http_verb = strdup("POST");
    auth_req.http_url = msprintf("%s/auth/", SERVER_URI);
    j_body = json_pack("{ssss}", "username", USER_USERNAME, "password", USER_PASSWORD);
    ulfius_set_json_body_request(&auth_req, j_body);
    json_decref(j_body);
    res = ulfius_send_http_request(&auth_req, &auth_resp);
    if (res == U_OK && auth_resp.status == 200) {
      if (auth_resp.nb_cookies) {
        y_log_message(Y_LOG_LEVEL_DEBUG, "User %s authenticated", USER_USERNAME);
        cookie = msprintf("%s=%s", auth_resp.map_cookie[0].key, auth_resp.map_cookie[0].value);
        u_map_put(scope_req.map_header, "Cookie", cookie);
        u_map_put(user_req.map_header, "Cookie", cookie);
        o_free(cookie);

        scope_req.http_verb = strdup("PUT");
        scope_req.http_url = msprintf("%s/auth/grant/%s", SERVER_URI, CLIENT);
        j_body = json_pack("{ss}", "scope", SCOPE_LIST);
        ulfius_set_json_body_request(&scope_req, j_body);
        json_decref(j_body);
        if (ulfius_send_http_request(&scope_req, &scope_resp) != U_OK || scope_resp.status != 200) {
          y_log_message(Y_LOG_LEVEL_DEBUG, "Grant scope '%s' for %s error", CLIENT, SCOPE_LIST);
          do_test = 0;
        } else {
          y_log_message(Y_LOG_LEVEL_DEBUG, "Grant scope OK");
        }
        ulfius_clean_response(&scope_resp);
      }
    } else {
      y_log_message(Y_LOG_LEVEL_ERROR, "Error authentication user");
      do_test = 0;
    }
    ulfius_clean_response(&auth_resp);
    ulfius_clean_request(&auth_req);
  }

  if (do_test) {
    s = glewlwyd_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
  }

  j_body = json_pack("{ss}", "scope", "");
  ulfius_set_json_body_request(&scope_req, j_body);
  json_decref(j_body);
  if (ulfius_send_http_request(&scope_req, NULL) != U_OK) {
    y_log_message(Y_LOG_LEVEL_DEBUG, "Remove grant scope '%s' for %s error", CLIENT, SCOPE_LIST);
  }

  char * url = msprintf("%s/auth/", SERVER_URI);
  run_simple_test(&user_req, "DELETE", url, NULL, NULL, NULL, NULL, 200, NULL, NULL, NULL);
  o_free(url);

  ulfius_clean_request(&admin_req);
  ulfius_clean_request(&user_req);
  ulfius_clean_request(&scope_req);
  y_close_logs();

  return (do_test && number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
