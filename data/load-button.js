
var items = selecteditems.slice(0); // Create a duplicate of the selecteditems array for now

$(function () {
    var temp = {};
    ////////////////////////// Row 1 ///////////////////////////
    // menu button11
    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button11action0');
        temp[this.value] = this.subitems;
    });

    $('#button11action0')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button11value0');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button11action1');
        temp[this.value] = this.subitems;
    });

    $('#button11action1')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button11value1');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button11action2');
        temp[this.value] = this.subitems;
    });

    $('#button11action2')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button11value2');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    // menuButton 12
    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button12action0');
        temp[this.value] = this.subitems;
    });

    $('#button12action0')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button12value0');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button12action1');
        temp[this.value] = this.subitems;
    });

    $('#button12action1')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button12value1');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button12action2');
        temp[this.value] = this.subitems;
    });

    $('#button12action2')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button12value2');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    // menu Button13
    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button13action0');
        temp[this.value] = this.subitems;
    });

    $('#button13action0')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button13value0');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button13action1');
        temp[this.value] = this.subitems;
    });

    $('#button13action1')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button13value1');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button13action2');
        temp[this.value] = this.subitems;
    });

    $('#button13action2')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button13value2');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    // menu button14
    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button14action0');
        temp[this.value] = this.subitems;
    });

    $('#button14action0')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button14value0');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button14action1');
        temp[this.value] = this.subitems;
    });

    $('#button14action1')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button14value1');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button14action2');
        temp[this.value] = this.subitems;
    });

    $('#button14action2')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button14value2');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    ////////////////////////// Row 2 ///////////////////////////
    // menu button21
    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button21action0');
        temp[this.value] = this.subitems;
    });

    $('#button21action0')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button21value0');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button21action1');
        temp[this.value] = this.subitems;
    });

    $('#button21action1')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button21value1');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button21action2');
        temp[this.value] = this.subitems;
    });

    $('#button21action2')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button21value2');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    // menuButton 12
    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button22action0');
        temp[this.value] = this.subitems;
    });

    $('#button22action0')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button22value0');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button22action1');
        temp[this.value] = this.subitems;
    });

    $('#button22action1')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button22value1');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button22action2');
        temp[this.value] = this.subitems;
    });

    $('#button22action2')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button22value2');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    // menu Button23
    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button23action0');
        temp[this.value] = this.subitems;
    });

    $('#button23action0')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button23value0');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button23action1');
        temp[this.value] = this.subitems;
    });

    $('#button23action1')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button23value1');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button23action2');
        temp[this.value] = this.subitems;
    });

    $('#button23action2')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button23value2');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    // menu button24
    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button24action0');
        temp[this.value] = this.subitems;
    });

    $('#button24action0')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button24value0');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button24action1');
        temp[this.value] = this.subitems;
    });

    $('#button24action1')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button24value1');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button24action2');
        temp[this.value] = this.subitems;
    });

    $('#button24action2')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button24value2');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    ////////////////////////// Row 3 ///////////////////////////
    // menu button31
    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button31action0');
        temp[this.value] = this.subitems;
    });

    $('#button31action0')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button31value0');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button31action1');
        temp[this.value] = this.subitems;
    });

    $('#button31action1')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button31value1');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button31action2');
        temp[this.value] = this.subitems;
    });

    $('#button31action2')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button31value2');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    // menuButton 12
    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button32action0');
        temp[this.value] = this.subitems;
    });

    $('#button32action0')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button32value0');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button32action1');
        temp[this.value] = this.subitems;
    });

    $('#button32action1')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button32value1');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button32action2');
        temp[this.value] = this.subitems;
    });

    $('#button32action2')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button32value2');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    // menu Button33
    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button33action0');
        temp[this.value] = this.subitems;
    });

    $('#button33action0')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button33value0');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button33action1');
        temp[this.value] = this.subitems;
    });

    $('#button33action1')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button33value1');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button33action2');
        temp[this.value] = this.subitems;
    });

    $('#button33action2')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button33value2');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    // menu button34
    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button34action0');
        temp[this.value] = this.subitems;
    });

    $('#button34action0')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button34value0');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button34action1');
        temp[this.value] = this.subitems;
    });

    $('#button34action1')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button34value1');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();

    $.each(items, function () {
        $('<option />')
            .attr('value', this.value)
            .html(this.name)
            .appendTo('#button34action2');
        temp[this.value] = this.subitems;
    });

    $('#button34action2')
        .change(function () {
            var value = $(this).val();
            var menu = $('#button34value2');

            menu.empty();
            $.each(temp[value], function () {
                $('<option />')
                    .attr('value', this.value)
                    .html(this.name)
                    .appendTo(menu);
            });
        })
        .change();


});



