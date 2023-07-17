KV to Keyboard
=========

This app reads a key's value from the KV store and then writes it as the output
of a USB keyboard.

Creating The Key
----------------

You need to create the key-value pair to read from. You can do this with
tockloader:

```
tockloader tickv append k2k mymsgtosend --write-id 10
```

Configuring this TBF
--------------------

Be sure you have given read permission for the KV store to this app. After
compiling, run:

```
tockloader tbf tlv add persistent_acl 10 10 10
```

to set the write ID, read ID, and append ID to 10 for this app.
