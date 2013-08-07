# Sample groonga tokenizer

## Install

Install libgroonga-dev

Build this tokenizer.

    % ./configure
    % make
    % sudo make install

## Usage

Register `tokenizers/sample`:

    % groonga DB
    > register tokenizers/sample

Now, you can use `TokenSample` that tokenizes the content by a
character:

    > tokenizer TokenSample "abc d e f"
    [
      [
        0,
        0.0,
        0.0
      ],
      [
        {
          "value": "a",
          "position": 0
        },
        {
          "value": "b",
          "position": 1
        },
        {
          "value": "c",
          "position": 2
        },
        {
          "value": " ",
          "position": 3
        },
        {
          "value": "d",
          "position": 4
        },
        {
          "value": " ",
          "position": 5
        },
        {
          "value": "e",
          "position": 6
        },
        {
          "value": " ",
          "position": 7
        },
        {
          "value": "f",
          "position": 8
        }
      ]
    ]

## License

Public domain. You can copy and modify this project freely.
