/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __SECURITY_H__
#define __SECURITY_H__
 
#include <inttypes.h>
 
#define MBED_DOMAIN "27061846-db35-4ec3-969f-8fa3326fb513"
#define MBED_ENDPOINT_NAME "0fdea4d4-114e-4724-bec3-0038e23cd69d"
 
const uint8_t SERVER_CERT[] = "-----BEGIN CERTIFICATE-----\r\n"
"MIIBmDCCAT6gAwIBAgIEVUCA0jAKBggqhkjOPQQDAjBLMQswCQYDVQQGEwJGSTEN\r\n"
"MAsGA1UEBwwET3VsdTEMMAoGA1UECgwDQVJNMQwwCgYDVQQLDANJb1QxETAPBgNV\r\n"
"BAMMCEFSTSBtYmVkMB4XDTE1MDQyOTA2NTc0OFoXDTE4MDQyOTA2NTc0OFowSzEL\r\n"
"MAkGA1UEBhMCRkkxDTALBgNVBAcMBE91bHUxDDAKBgNVBAoMA0FSTTEMMAoGA1UE\r\n"
"CwwDSW9UMREwDwYDVQQDDAhBUk0gbWJlZDBZMBMGByqGSM49AgEGCCqGSM49AwEH\r\n"
"A0IABLuAyLSk0mA3awgFR5mw2RHth47tRUO44q/RdzFZnLsAsd18Esxd5LCpcT9w\r\n"
"0tvNfBv4xJxGw0wcYrPDDb8/rjujEDAOMAwGA1UdEwQFMAMBAf8wCgYIKoZIzj0E\r\n"
"AwIDSAAwRQIhAPAonEAkwixlJiyYRQQWpXtkMZax+VlEiS201BG0PpAzAiBh2RsD\r\n"
"NxLKWwf4O7D6JasGBYf9+ZLwl0iaRjTjytO+Kw==\r\n"
"-----END CERTIFICATE-----\r\n";
 
const uint8_t CERT[] = "-----BEGIN CERTIFICATE-----\r\n"
"MIIBzjCCAXOgAwIBAgIEJzMWqDAMBggqhkjOPQQDAgUAMDkxCzAJBgNVBAYTAkZ\r\n"
"JMQwwCgYDVQQKDANBUk0xHDAaBgNVBAMME21iZWQtY29ubmVjdG9yLTIwMTgwHh\r\n"
"cNMTcwMzAyMTkyMjI4WhcNMTgxMjMxMDYwMDAwWjCBoTFSMFAGA1UEAxNJMjcwN\r\n"
"jE4NDYtZGIzNS00ZWMzLTk2OWYtOGZhMzMyNmZiNTEzLzBmZGVhNGQ0LTExNGUt\r\n"
"NDcyNC1iZWMzLTAwMzhlMjNjZDY5ZDEMMAoGA1UECxMDQVJNMRIwEAYDVQQKEwl\r\n"
"tYmVkIHVzZXIxDTALBgNVBAcTBE91bHUxDTALBgNVBAgTBE91bHUxCzAJBgNVBA\r\n"
"YTAkZJMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEfT6kYtsP3TLbUYzQ5x+FW\r\n"
"P9ZQnEfNgBoWP90SRwj6Y+7xoXH0OcWe6PnkpG81NeCJe1jv0kcVVQPN2rUXpso\r\n"
"FDAMBggqhkjOPQQDAgUAA0cAMEQCIEX+IVaWjkdhPgvpa2L/YDxA2sN4YmAiZRS\r\n"
"DUvBIo70oAiBKGItrDzoeADVMsk/eBp53zBbI+RYVrQzvy2Ab5wjtxA==\r\n"
"-----END CERTIFICATE-----\r\n";
 
const uint8_t KEY[] = "-----BEGIN PRIVATE KEY-----\r\n"
"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQg1TB2+RdTbrroiK70\r\n"
"QpnGCl8xjpgtnBn2CIHBauS4fD2hRANCAAR9PqRi2w/dMttRjNDnH4VY/1lCcR82\r\n"
"AGhY/3RJHCPpj7vGhcfQ5xZ7o+eSkbzU14Il7WO/SRxVVA83atRemygU\r\n"
"-----END PRIVATE KEY-----\r\n";
 
#endif //__SECURITY_H__