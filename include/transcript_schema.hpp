#ifndef TRANSCRIPT_SCHEMA_HPP
#define TRANSCRIPT_SCHEMA_HPP

static constexpr auto transcript_schema = R"(
{
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "type": "object",
    "properties": {
        "transcripts": {
            "type": "array",
            "items": [
                {"$ref": "#/$defs/2^12SubTranscript"},
                {"$ref": "#/$defs/2^13SubTranscript"},
                {"$ref": "#/$defs/2^14SubTranscript"},
                {"$ref": "#/$defs/2^15SubTranscript"}
            ],
            "minItems": 4,
            "maxItems": 4
        },
        "participantIds": {
            "type": "array",
            "items": {
                "oneOf": [
                    {"$ref": "#/$defs/ethereumId"},
                    {"$ref": "#/$defs/githubId"},
                    {"$ref": "#/$defs/emptyString"}
                ]
            }
        },
        "participantEcdsaSignatures": {
            "type": "array",
            "items": {
                "oneOf": [
                    {"$ref": "#/$defs/ecdsaSignature"},
                    {"$ref": "#/$defs/emptyString"}
                ]
            }
        }
    },
    "$defs": {
        "emptyString": {
            "type": "string",
            "pattern": "^$"
        },
        "G1Point": {
            "type": "string",
            "pattern": "^0x[a-f0-9]{96}$"
        },
        "G2Point": {
            "type": "string",
            "pattern": "^0x[a-f0-9]{192}$"
        },
        "ethereumId": {
            "type": "string",
            "pattern": "^eth\\|0x[a-f\\d]{40}$"
        },
        "githubId": {
            "type": "string",
            "pattern": "^git\\|\\d{1,16}\\|[a-zA-Z\\d](?:[a-zA-Z\\d]|-(?=[a-zA-Z\\d])){0,38}$"
        },
        "ecdsaSignature": {
            "type": "string",
            "pattern": "^0x[a-f\\d]{130}$"
        },
        "2^12SubTranscript": {
            "type": "object",
            "properties": {
                "numG1Powers": {
                    "type": "integer",
                    "minimum": 4096,
                    "maximum": 4096
                },
                "numG2Powers": {
                    "type": "integer",
                    "minimum": 65,
                    "maximum": 65
                },
                "powersOfTau": {
                    "type": "object",
                    "properties": {
                        "G1Powers": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G1Point"
                            },
                            "minItems": 4096,
                            "maxItems": 4096
                        },
                        "G2Powers": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G2Point"
                            },
                            "minItems": 65,
                            "maxItems": 65
                        }
                    },
                    "required": [
                        "G1Powers",
                        "G2Powers"
                    ]
                },
                "witness": {
                    "type": "object",
                    "properties": {
                        "runningProducts": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G1Point"
                            }
                        },
                        "potPubkeys": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G2Point"
                            }
                        },
                        "bls_signature": {
                            "type": "array",
                            "items": {
                                "oneOf": [
                                    {"$ref": "#/$defs/G1Point"},
                                    {"$ref": "#/$defs/emptyString"}
                                ]
                            }
                        }
                    },
                    "required": [
                        "runningProducts",
                        "potPubkeys",
                        "bls_signature"
                    ]
                }
            },
            "required": [
                "numG1Powers",
                "numG2Powers",
                "powersOfTau",
                "witness"
            ]
        },
        "2^13SubTranscript": {
            "type": "object",
            "properties": {
                "numG1Powers": {
                    "type": "integer",
                    "minimum": 8192,
                    "maximum": 8192
                },
                "numG2Powers": {
                    "type": "integer",
                    "minimum": 65,
                    "maximum": 65
                },
                "powersOfTau": {
                    "type": "object",
                    "properties": {
                        "G1Powers": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G1Point"
                            },
                            "minItems": 8192,
                            "maxItems": 8192
                        },
                        "G2Powers": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G2Point"
                            },
                            "minItems": 65,
                            "maxItems": 65
                        }
                    },
                    "required": [
                        "G1Powers",
                        "G2Powers"
                    ]
                },
                "witness": {
                    "type": "object",
                    "properties": {
                        "runningProducts": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G1Point"
                            }
                        },
                        "potPubkeys": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G2Point"
                            }
                        },
                        "bls_signature": {
                            "type": "array",
                            "items": {
                                "oneOf": [
                                    {"$ref": "#/$defs/G1Point"},
                                    {"$ref": "#/$defs/emptyString"}
                                ]
                            }
                        }
                    },
                    "required": [
                        "runningProducts",
                        "potPubkeys",
                        "bls_signature"
                    ]
                }
            },
            "required": [
                "numG1Powers",
                "numG2Powers",
                "powersOfTau",
                "witness"
            ]
        },
        "2^14SubTranscript": {
            "type": "object",
            "properties": {
                "numG1Powers": {
                    "type": "integer",
                    "minimum": 16384,
                    "maximum": 16384
                },
                "numG2Powers": {
                    "type": "integer",
                    "minimum": 65,
                    "maximum": 65
                },
                "powersOfTau": {
                    "type": "object",
                    "properties": {
                        "G1Powers": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G1Point"
                            },
                            "minItems": 16384,
                            "maxItems": 16384
                        },
                        "G2Powers": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G2Point"
                            },
                            "minItems": 65,
                            "maxItems": 65
                        }
                    },
                    "required": [
                        "G1Powers",
                        "G2Powers"
                    ]
                },
                "witness": {
                    "type": "object",
                    "properties": {
                        "runningProducts": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G1Point"
                            }
                        },
                        "potPubkeys": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G2Point"
                            }
                        },
                        "bls_signature": {
                            "type": "array",
                            "items": {
                                "oneOf": [
                                    {"$ref": "#/$defs/G1Point"},
                                    {"$ref": "#/$defs/emptyString"}
                                ]
                            }
                        }
                    },
                    "required": [
                        "runningProducts",
                        "potPubkeys",
                        "bls_signature"
                    ]
                }
            },
            "required": [
                "numG1Powers",
                "numG2Powers",
                "powersOfTau",
                "witness"
            ]
        },
        "2^15SubTranscript": {
            "type": "object",
            "properties": {
                "numG1Powers": {
                    "type": "integer",
                    "minimum": 32768,
                    "maximum": 32768
                },
                "numG2Powers": {
                    "type": "integer",
                    "minimum": 65,
                    "maximum": 65
                },
                "powersOfTau": {
                    "type": "object",
                    "properties": {
                        "G1Powers": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G1Point"
                            },
                            "minItems": 32768,
                            "maxItems": 32768
                        },
                        "G2Powers": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G2Point"
                            },
                            "minItems": 65,
                            "maxItems": 65
                        }
                    },
                    "required": [
                        "G1Powers",
                        "G2Powers"
                    ]
                },
                "witness": {
                    "type": "object",
                    "properties": {
                        "runningProducts": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G1Point"
                            }
                        },
                        "potPubkeys": {
                            "type": "array",
                            "items": {
                                "$ref": "#/$defs/G2Point"
                            }
                        },
                        "bls_signature": {
                            "type": "array",
                            "items": {
                                "oneOf": [
                                    {"$ref": "#/$defs/G1Point"},
                                    {"$ref": "#/$defs/emptyString"}
                                ]
                            }
                        }
                    },
                    "required": [
                        "runningProducts",
                        "potPubkeys",
                        "bls_signature"
                    ]
                }
            },
            "required": [
                "numG1Powers",
                "numG2Powers",
                "powersOfTau",
                "witness"
            ]
        }
    }
}
)";

#endif // TRANSCRIPT_SCHEMA_HPP