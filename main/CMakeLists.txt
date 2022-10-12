idf_component_register(SRCS 
                    "main.c"
                    INCLUDE_DIRS 
                    "."
                    PRIV_REQUIRES main event component_a component_b events_description)
