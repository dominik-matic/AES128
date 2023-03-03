# AES128

I implemented AES128 ECB and CTR for fun. Do not use this for anything serious, it's probably full of security flaws.

Usage:

    main {enc|dec} {ecb|ctr} input_filename output_filename key_file iv_file

key_file and iv_file are text files containing a hex representation of a key or IV.
iv_file is only necessary when using ctr mode,
