UTC_data = {
    "0": {
        'offset': -12,
        'zone-description': 'Baker Island, Howland Island (both uninhabited)'},
    "1": {
        'offset': -11,
        'zone-description': 'American Samoa, Samoa'},
    "2": {
        'offset': -10,
        'zone-description': '(Summer)French Polynesia (most), United States (Aleutian Islands, Hawaii)'},
    "3": {
        'offset': -9.5,
        'zone-description': 'Marquesas Islands'},
    "4": {
        'offset': -9,
        'zone-description': 'Gambier Islands;(Summer)United States (most of Alaska)'},
    "5": {
        'offset': -8,
        'zone-description': '(Summer)Canada (most of British Columbia), Mexico (Baja California)'},
    "6": {
        'offset': -8,
        'zone-description': 'United States (California, most of Nevada, most of Oregon, Washington (state))'},
    "7": {
        'offset': -7,
        'zone-description': 'Mexico (Sonora), United States (Arizona); (Summer)Canada (Alberta)'},
    "8": {
        'offset': -7,
        'zone-description': 'Mexico (Chihuahua), United States (Colorado)'},
    "9": {
        'offset': -6,
        'zone-description': ' Costa Rica, El Salvador, Ecuador (Galapagos Islands), Guatemala, Honduras'},
    "10": {
        'offset': -6,
        'zone-description': 'Mexico (most), Nicaragua;(Summer)Canada (Manitoba, Saskatchewan), United States (Illinois, most of Texas)'},
    "11": {
        'offset': -5,
        'zone-description': 'Colombia, Cuba, Ecuador (continental), Haiti, Jamaica, Panama, Peru'},
    "12": {
        'offset': -5,
        'zone-description': '(Summer)Canada (most of Ontario, most of Quebec)'},
    "13": {
        'offset': -5,
        'zone-description': 'United States (most of Florida, Georgia, Massachusetts, most of Michigan, New York, North Carolina, Ohio, Washington D.C.)'},
    "14": {
        'offset': -4.5,
        'zone-description': 'Venezuela'},
    "15": {
        'offset': -4,
        'zone-description': 'Bolivia, Brazil (Amazonas), Chile (continental), Dominican Republic, Canada (Nova Scotia), Paraguay'},
    "16": {
        'offset': -4,
        'zone-description': 'Puerto Rico, Trinidad and Tobago'},
    "17": {
        'offset': -3.5,
        'zone-description': 'Canada (Newfoundland)'},
    "18": {
        'offset': -3,
        'zone-description': 'Argentina; (Summer) Brazil (Brasilia, Rio de Janeiro, Sao Paulo), most of Greenland, Uruguay'},
    "19": {
        'offset': -2,
        'zone-description': 'Brazil (Fernando de Noronha), South Georgia and the South Sandwich Islands'},
    "20": {
        'offset': -1,
        'zone-description': 'Portugal (Azores), Cape Verde'},
    "21": {
        'offset': 0,
        'zone-description': "Cote d'Ivoire, Faroe Islands, Ghana, Iceland, Senegal; (Summer) Ireland, Portugal (continental and Madeira)"},
    "22": {
        'offset': 0,
        'zone-description': 'Spain (Canary Islands), Morocco, United Kingdom'},
    "23": {
        'offset': 1,
        'zone-description': "Angola, Cameroon, Nigeria, Tunisia; (Summer)Albania, Algeria, Austria, Belgium, Bosnia and Herzegovina,"},
    "24": {
        'offset': 1,
        'zone-description': "Spain (continental), Croatia, Czech Republic, Denmark, Germany, Hungary, Italy, Kinshasa, Kosovo,"},
    "25": {
        'offset': 1,
        'zone-description': "Macedonia, France (metropolitan), the Netherlands, Norway, Poland, Serbia, Slovakia, Slovenia, Sweden, Switzerland"},
    "26": {
        'offset': 2,
        'zone-description': "Libya, Egypt, Malawi, Mozambique, South Africa, Zambia, Zimbabwe, (Summer)Bulgaria, Cyprus, Estonia,"},
    "27": {
        'offset': 2,
        'zone-description': "Finland, Greece, Israel, Jordan, Latvia, Lebanon, Lithuania, Moldova, Palestine, Romania, Syria, Turkey, Ukraine"},
    "28": {
        'offset': 3,
        'zone-description': "Belarus, Djibouti, Eritrea, Ethiopia, Iraq, Kenya, Madagascar, Russia (Kaliningrad Oblast), Saudi Arabia,"},
    "29": {
        'offset': 3,
        'zone-description': "South Sudan, Sudan, Somalia, South Sudan, Tanzania, Uganda, Yemen"},
    "30": {
        'offset': 3.5,
        'zone-description': "(Summer)Iran"},
    "31": {
        'offset': 4,
        'zone-description': "Armenia, Azerbaijan, Georgia, Mauritius, Oman, Russia (European), Seychelles, United Arab Emirates"},
    "32": {
        'offset': 4.5,
        'zone-description': "Afghanistan"},
    "33": {
        'offset': 5,
        'zone-description': "Kazakhstan (West), Maldives, Pakistan, Uzbekistan"},
    "34": {
        'offset': 5.5,
        'zone-description': "India, Sri Lanka"},
    "35": {
        'offset': 5.75,
        'zone-description': "Nepal"},
    "36": {
        'offset': 6,
        'zone-description': "Kazakhstan (most), Bangladesh, Russia (Ural: Sverdlovsk Oblast, Chelyabinsk Oblast)"},
    "37": {
        'offset': 6.5,
        'zone-description': "Cocos Islands, Myanmar"},
    "38": {
        'offset': 7,
        'zone-description': "Jakarta, Russia (Novosibirsk Oblast), Thailand, Vietnam"},
    "39": {
        'offset': 8,
        'zone-description': "China, Hong Kong, Russia (Krasnoyarsk Krai), Malaysia, Philippines, Singapore, Taiwan, most of Mongolia, Western Australia"},
    "40": {
        'offset': 9,
        'zone-description': "Korea, East Timor, Russia (Irkutsk Oblast), Japan"},
    "41": {
        'offset': 9.5,
        'zone-description': "Australia (Northern Territory);(Summer)Australia (South Australia))"},
    "42": {
        'offset': 10,
        'zone-description': "Russia (Zabaykalsky Krai); (Summer)Australia (New South Wales, Queensland, Tasmania, Victoria)"},
    "43": {
        'offset': 10.5,
        'zone-description': "Lord Howe Island"},
    "44": {
        'offset': 11,
        'zone-description': "New Caledonia, Russia (Primorsky Krai), Solomon Islands"},
    "45": {
        'offset': 11.5,
        'zone-description': "Norfolk Island"},
    "46": {
        'offset': 12,
        'zone-description': "Fiji, Russia (Kamchatka Krai);(Summer)New Zealand"},
    "47": {
        'offset': 12.75,
        'zone-description': "(Summer)New Zealand"},
    "48": {
        'offset': 13,
        'zone-description': "Tonga"},
    "49": {
        'offset': 14,
        'zone-description': "Kiribati (Line Islands)"},

}

print('// UTC enumeration')
print('typedef enum UTC_timezone_t{')
for key,value in UTC_data.items():
    offset = value['offset']
    description = value['zone-description']
    neg: str = 'n' if offset < 0 else ''
    frac = int((offset % 1)*60)
    offset_int = int(offset)
    print('\tUTC_{}{:02d}_{:02d} = {},\t// {}'.format(neg, abs(offset_int), frac, key,description))
print('} UTC_timezone_t;\n')

print('// UTC offsets in seconds')
print('const int32_t UTC_offsets_s[{}] = {{'.format(len(UTC_data)))
for key, value in UTC_data.items():
    offset = value['offset']
    offset_seconds = int(offset*3600)
    endchar = ' ' if int(key) == (len(UTC_data)-1) else ', '
    print('{}'.format(offset_seconds), end=endchar)
print('};\n')

# print('// UTC offsets in hours')
# print('const float UTC_offsets_h[{}] = {{'.format(len(UTC_data)))
# for key, value in UTC_data.items():
#     offset = value['offset']
#     endchar = ' ' if int(key) == (len(UTC_data)-1) else ', '
#     print('{}'.format(offset), end=endchar)
# print('};\n')

