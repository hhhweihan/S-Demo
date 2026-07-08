
bomb:     file format elf64-x86-64


Disassembly of section .interp:

0000000000000238 <.interp>:
 238:	2f                   	(bad)  
 239:	6c                   	insb   (%dx),%es:(%rdi)
 23a:	69 62 36 34 2f 6c 64 	imul   $0x646c2f34,0x36(%rdx),%esp
 241:	2d 6c 69 6e 75       	sub    $0x756e696c,%eax
 246:	78 2d                	js     275 <_init-0xb5b>
 248:	78 38                	js     282 <_init-0xb4e>
 24a:	36 2d 36 34 2e 73    	ss sub $0x732e3436,%eax
 250:	6f                   	outsl  %ds:(%rsi),(%dx)
 251:	2e 32 00             	xor    %cs:(%rax),%al

Disassembly of section .note.ABI-tag:

0000000000000254 <.note.ABI-tag>:
 254:	04 00                	add    $0x0,%al
 256:	00 00                	add    %al,(%rax)
 258:	10 00                	adc    %al,(%rax)
 25a:	00 00                	add    %al,(%rax)
 25c:	01 00                	add    %eax,(%rax)
 25e:	00 00                	add    %al,(%rax)
 260:	47                   	rex.RXB
 261:	4e 55                	rex.WRX push %rbp
 263:	00 00                	add    %al,(%rax)
 265:	00 00                	add    %al,(%rax)
 267:	00 03                	add    %al,(%rbx)
 269:	00 00                	add    %al,(%rax)
 26b:	00 02                	add    %al,(%rdx)
 26d:	00 00                	add    %al,(%rax)
 26f:	00 00                	add    %al,(%rax)
 271:	00 00                	add    %al,(%rax)
	...

Disassembly of section .note.gnu.build-id:

0000000000000274 <.note.gnu.build-id>:
 274:	04 00                	add    $0x0,%al
 276:	00 00                	add    %al,(%rax)
 278:	14 00                	adc    $0x0,%al
 27a:	00 00                	add    %al,(%rax)
 27c:	03 00                	add    (%rax),%eax
 27e:	00 00                	add    %al,(%rax)
 280:	47                   	rex.RXB
 281:	4e 55                	rex.WRX push %rbp
 283:	00 c1                	add    %al,%cl
 285:	11 9d 17 57 41 9b    	adc    %ebx,-0x64bea8e9(%rbp)
 28b:	c8 68 70 1e          	enterq $0x7068,$0x1e
 28f:	1c bf                	sbb    $0xbf,%al
 291:	4b                   	rex.WXB
 292:	a1                   	.byte 0xa1
 293:	99                   	cltd   
 294:	42                   	rex.X
 295:	f2 b5 e3             	repnz mov $0xe3,%ch

Disassembly of section .gnu.hash:

0000000000000298 <.gnu.hash>:
 298:	03 00                	add    (%rax),%eax
 29a:	00 00                	add    %al,(%rax)
 29c:	1f                   	(bad)  
 29d:	00 00                	add    %al,(%rax)
 29f:	00 01                	add    %al,(%rcx)
 2a1:	00 00                	add    %al,(%rax)
 2a3:	00 06                	add    %al,(%rsi)
 2a5:	00 00                	add    %al,(%rax)
 2a7:	00 00                	add    %al,(%rax)
 2a9:	01 20                	add    %esp,(%rax)
 2ab:	00 80 01 10 02 1f    	add    %al,0x1f021001(%rax)
 2b1:	00 00                	add    %al,(%rax)
 2b3:	00 20                	add    %ah,(%rax)
 2b5:	00 00                	add    %al,(%rax)
 2b7:	00 00                	add    %al,(%rax)
 2b9:	00 00                	add    %al,(%rax)
 2bb:	00 29                	add    %ch,(%rcx)
 2bd:	1d 8c 1c 66 55       	sbb    $0x55661c8c,%eax
 2c2:	61                   	(bad)  
 2c3:	10 39                	adc    %bh,(%rcx)
 2c5:	f2                   	repnz
 2c6:	8b                   	.byte 0x8b
 2c7:	1c                   	.byte 0x1c

Disassembly of section .dynsym:

00000000000002c8 <.dynsym>:
	...
 2e0:	c7 00 00 00 12 00    	movl   $0x120000,(%rax)
	...
 2f6:	00 00                	add    %al,(%rax)
 2f8:	80 00 00             	addb   $0x0,(%rax)
 2fb:	00 12                	add    %dl,(%rdx)
	...
 30d:	00 00                	add    %al,(%rax)
 30f:	00 60 01             	add    %ah,0x1(%rax)
 312:	00 00                	add    %al,(%rax)
 314:	20 00                	and    %al,(%rax)
	...
 326:	00 00                	add    %al,(%rax)
 328:	19 00                	sbb    %eax,(%rax)
 32a:	00 00                	add    %al,(%rax)
 32c:	12 00                	adc    (%rax),%al
	...
 33e:	00 00                	add    %al,(%rax)
 340:	57                   	push   %rdi
 341:	00 00                	add    %al,(%rax)
 343:	00 12                	add    %dl,(%rdx)
	...
 355:	00 00                	add    %al,(%rax)
 357:	00 24 01             	add    %ah,(%rcx,%rax,1)
 35a:	00 00                	add    %al,(%rax)
 35c:	12 00                	adc    (%rax),%al
	...
 36e:	00 00                	add    %al,(%rax)
 370:	5c                   	pop    %rsp
 371:	00 00                	add    %al,(%rax)
 373:	00 12                	add    %dl,(%rdx)
	...
 385:	00 00                	add    %al,(%rax)
 387:	00 d5                	add    %dl,%ch
 389:	00 00                	add    %al,(%rax)
 38b:	00 12                	add    %dl,(%rdx)
	...
 39d:	00 00                	add    %al,(%rax)
 39f:	00 e9                	add    %ch,%cl
 3a1:	00 00                	add    %al,(%rax)
 3a3:	00 12                	add    %dl,(%rdx)
	...
 3b5:	00 00                	add    %al,(%rax)
 3b7:	00 91 00 00 00 12    	add    %dl,0x12000000(%rcx)
	...
 3cd:	00 00                	add    %al,(%rax)
 3cf:	00 12                	add    %dl,(%rdx)
 3d1:	01 00                	add    %eax,(%rax)
 3d3:	00 12                	add    %dl,(%rdx)
	...
 3e5:	00 00                	add    %al,(%rax)
 3e7:	00 7a 00             	add    %bh,0x0(%rdx)
 3ea:	00 00                	add    %al,(%rax)
 3ec:	12 00                	adc    (%rax),%al
	...
 3fe:	00 00                	add    %al,(%rax)
 400:	50                   	push   %rax
 401:	00 00                	add    %al,(%rax)
 403:	00 12                	add    %dl,(%rdx)
	...
 415:	00 00                	add    %al,(%rax)
 417:	00 db                	add    %bl,%bl
 419:	00 00                	add    %al,(%rax)
 41b:	00 12                	add    %dl,(%rdx)
	...
 42d:	00 00                	add    %al,(%rax)
 42f:	00 ab 00 00 00 12    	add    %ch,0x12000000(%rbx)
	...
 445:	00 00                	add    %al,(%rax)
 447:	00 7c 01 00          	add    %bh,0x0(%rcx,%rax,1)
 44b:	00 20                	add    %ah,(%rax)
	...
 45d:	00 00                	add    %al,(%rax)
 45f:	00 73 00             	add    %dh,0x0(%rbx)
 462:	00 00                	add    %al,(%rax)
 464:	12 00                	adc    (%rax),%al
	...
 476:	00 00                	add    %al,(%rax)
 478:	12 00                	adc    (%rax),%al
 47a:	00 00                	add    %al,(%rax)
 47c:	12 00                	adc    (%rax),%al
	...
 48e:	00 00                	add    %al,(%rax)
 490:	38 00                	cmp    %al,(%rax)
 492:	00 00                	add    %al,(%rax)
 494:	12 00                	adc    (%rax),%al
	...
 4a6:	00 00                	add    %al,(%rax)
 4a8:	20 00                	and    %al,(%rax)
 4aa:	00 00                	add    %al,(%rax)
 4ac:	12 00                	adc    (%rax),%al
	...
 4be:	00 00                	add    %al,(%rax)
 4c0:	32 00                	xor    (%rax),%al
 4c2:	00 00                	add    %al,(%rax)
 4c4:	12 00                	adc    (%rax),%al
	...
 4d6:	00 00                	add    %al,(%rax)
 4d8:	2d 00 00 00 12       	sub    $0x12000000,%eax
	...
 4ed:	00 00                	add    %al,(%rax)
 4ef:	00 48 00             	add    %cl,0x0(%rax)
 4f2:	00 00                	add    %al,(%rax)
 4f4:	12 00                	adc    (%rax),%al
	...
 506:	00 00                	add    %al,(%rax)
 508:	96                   	xchg   %eax,%esi
 509:	00 00                	add    %al,(%rax)
 50b:	00 12                	add    %dl,(%rdx)
	...
 51d:	00 00                	add    %al,(%rax)
 51f:	00 8b 01 00 00 20    	add    %cl,0x20000001(%rbx)
	...
 535:	00 00                	add    %al,(%rax)
 537:	00 ef                	add    %ch,%bh
 539:	00 00                	add    %al,(%rax)
 53b:	00 12                	add    %dl,(%rdx)
	...
 54d:	00 00                	add    %al,(%rax)
 54f:	00 f5                	add    %dh,%ch
 551:	00 00                	add    %al,(%rax)
 553:	00 22                	add    %ah,(%rdx)
	...
 565:	00 00                	add    %al,(%rax)
 567:	00 b9 00 00 00 12    	add    %bh,0x12000000(%rcx)
	...
 57d:	00 00                	add    %al,(%rax)
 57f:	00 04 01             	add    %al,(%rcx,%rax,1)
 582:	00 00                	add    %al,(%rax)
 584:	12 00                	adc    (%rax),%al
	...
 596:	00 00                	add    %al,(%rax)
 598:	0b 00                	or     (%rax),%eax
 59a:	00 00                	add    %al,(%rax)
 59c:	12 00                	adc    (%rax),%al
	...
 5ae:	00 00                	add    %al,(%rax)
 5b0:	a4                   	movsb  %ds:(%rsi),%es:(%rdi)
 5b1:	00 00                	add    %al,(%rax)
 5b3:	00 11                	add    %dl,(%rcx)
 5b5:	00 18                	add    %bl,(%rax)
 5b7:	00 60 46             	add    %ah,0x46(%rax)
 5ba:	20 00                	and    %al,(%rax)
 5bc:	00 00                	add    %al,(%rax)
 5be:	00 00                	add    %al,(%rax)
 5c0:	08 00                	or     %al,(%rax)
 5c2:	00 00                	add    %al,(%rax)
 5c4:	00 00                	add    %al,(%rax)
 5c6:	00 00                	add    %al,(%rax)
 5c8:	6d                   	insl   (%dx),%es:(%rdi)
 5c9:	00 00                	add    %al,(%rax)
 5cb:	00 11                	add    %dl,(%rcx)
 5cd:	00 18                	add    %bl,(%rax)
 5cf:	00 70 46             	add    %dh,0x46(%rax)
 5d2:	20 00                	and    %al,(%rax)
 5d4:	00 00                	add    %al,(%rax)
 5d6:	00 00                	add    %al,(%rax)
 5d8:	08 00                	or     %al,(%rax)
 5da:	00 00                	add    %al,(%rax)
 5dc:	00 00                	add    %al,(%rax)
 5de:	00 00                	add    %al,(%rax)
 5e0:	ce                   	(bad)  
 5e1:	00 00                	add    %al,(%rax)
 5e3:	00 11                	add    %dl,(%rcx)
 5e5:	00 18                	add    %bl,(%rax)
 5e7:	00 80 46 20 00 00    	add    %al,0x2046(%rax)
 5ed:	00 00                	add    %al,(%rax)
 5ef:	00 08                	add    %cl,(%rax)
 5f1:	00 00                	add    %al,(%rax)
 5f3:	00 00                	add    %al,(%rax)
 5f5:	00 00                	add    %al,(%rax)
	...

Disassembly of section .dynstr:

00000000000005f8 <.dynstr>:
 5f8:	00 6c 69 62          	add    %ch,0x62(%rcx,%rbp,2)
 5fc:	63 2e                	movslq (%rsi),%ebp
 5fe:	73 6f                	jae    66f <_init-0x761>
 600:	2e 36 00 73 6f       	cs add %dh,%ss:0x6f(%rbx)
 605:	63 6b 65             	movslq 0x65(%rbx),%ebp
 608:	74 00                	je     60a <_init-0x7c6>
 60a:	66 66 6c             	data16 data16 insb (%dx),%es:(%rdi)
 60d:	75 73                	jne    682 <_init-0x74e>
 60f:	68 00 73 74 72       	pushq  $0x72747300
 614:	63 70 79             	movslq 0x79(%rax),%esi
 617:	00 5f 5f             	add    %bl,0x5f(%rdi)
 61a:	70 72                	jo     68e <_init-0x742>
 61c:	69 6e 74 66 5f 63 68 	imul   $0x68635f66,0x74(%rsi),%ebp
 623:	6b 00 65             	imul   $0x65,(%rax),%eax
 626:	78 69                	js     691 <_init-0x73f>
 628:	74 00                	je     62a <_init-0x7a6>
 62a:	66 6f                	outsw  %ds:(%rsi),(%dx)
 62c:	70 65                	jo     693 <_init-0x73d>
 62e:	6e                   	outsb  %ds:(%rsi),(%dx)
 62f:	00 5f 5f             	add    %bl,0x5f(%rdi)
 632:	69 73 6f 63 39 39 5f 	imul   $0x5f393963,0x6f(%rbx),%esi
 639:	73 73                	jae    6ae <_init-0x722>
 63b:	63 61 6e             	movslq 0x6e(%rcx),%esp
 63e:	66 00 63 6f          	data16 add %ah,0x6f(%rbx)
 642:	6e                   	outsb  %ds:(%rsi),(%dx)
 643:	6e                   	outsb  %ds:(%rsi),(%dx)
 644:	65 63 74 00 73       	movslq %gs:0x73(%rax,%rax,1),%esi
 649:	69 67 6e 61 6c 00 70 	imul   $0x70006c61,0x6e(%rdi),%esp
 650:	75 74                	jne    6c6 <_init-0x70a>
 652:	73 00                	jae    654 <_init-0x77c>
 654:	5f                   	pop    %rdi
 655:	5f                   	pop    %rdi
 656:	73 74                	jae    6cc <_init-0x704>
 658:	61                   	(bad)  
 659:	63 6b 5f             	movslq 0x5f(%rbx),%ebp
 65c:	63 68 6b             	movslq 0x6b(%rax),%ebp
 65f:	5f                   	pop    %rdi
 660:	66 61                	data16 (bad) 
 662:	69 6c 00 73 74 64 69 	imul   $0x6e696474,0x73(%rax,%rax,1),%ebp
 669:	6e 
 66a:	00 73 74             	add    %dh,0x74(%rbx)
 66d:	72 74                	jb     6e3 <_init-0x6ed>
 66f:	6f                   	outsl  %ds:(%rsi),(%dx)
 670:	6c                   	insb   (%dx),%es:(%rdi)
 671:	00 66 67             	add    %ah,0x67(%rsi)
 674:	65 74 73             	gs je  6ea <_init-0x6e6>
 677:	00 5f 5f             	add    %bl,0x5f(%rdi)
 67a:	65 72 72             	gs jb  6ef <_init-0x6e1>
 67d:	6e                   	outsb  %ds:(%rsi),(%dx)
 67e:	6f                   	outsl  %ds:(%rsi),(%dx)
 67f:	5f                   	pop    %rdi
 680:	6c                   	insb   (%dx),%es:(%rdi)
 681:	6f                   	outsl  %ds:(%rsi),(%dx)
 682:	63 61 74             	movslq 0x74(%rcx),%esp
 685:	69 6f 6e 00 72 65 61 	imul   $0x61657200,0x6e(%rdi),%ebp
 68c:	64 00 5f 5f          	add    %bl,%fs:0x5f(%rdi)
 690:	66 70 72             	data16 jo 705 <_init-0x6cb>
 693:	69 6e 74 66 5f 63 68 	imul   $0x68635f66,0x74(%rsi),%ebp
 69a:	6b 00 73             	imul   $0x73,(%rax),%eax
 69d:	74 64                	je     703 <_init-0x6cd>
 69f:	6f                   	outsl  %ds:(%rsi),(%dx)
 6a0:	75 74                	jne    716 <_init-0x6ba>
 6a2:	00 5f 5f             	add    %bl,0x5f(%rdi)
 6a5:	6d                   	insl   (%dx),%es:(%rdi)
 6a6:	65 6d                	gs insl (%dx),%es:(%rdi)
 6a8:	6d                   	insl   (%dx),%es:(%rdi)
 6a9:	6f                   	outsl  %ds:(%rsi),(%dx)
 6aa:	76 65                	jbe    711 <_init-0x6bf>
 6ac:	5f                   	pop    %rdi
 6ad:	63 68 6b             	movslq 0x6b(%rax),%ebp
 6b0:	00 5f 5f             	add    %bl,0x5f(%rdi)
 6b3:	63 74 79 70          	movslq 0x70(%rcx,%rdi,2),%esi
 6b7:	65 5f                	gs pop %rdi
 6b9:	62                   	(bad)  
 6ba:	5f                   	pop    %rdi
 6bb:	6c                   	insb   (%dx),%es:(%rdi)
 6bc:	6f                   	outsl  %ds:(%rsi),(%dx)
 6bd:	63 00                	movslq (%rax),%eax
 6bf:	67 65 74 65          	addr32 gs je 728 <_init-0x6a8>
 6c3:	6e                   	outsb  %ds:(%rsi),(%dx)
 6c4:	76 00                	jbe    6c6 <_init-0x70a>
 6c6:	73 74                	jae    73c <_init-0x694>
 6c8:	64 65 72 72          	fs gs jb 73e <_init-0x692>
 6cc:	00 61 6c             	add    %ah,0x6c(%rcx)
 6cf:	61                   	(bad)  
 6d0:	72 6d                	jb     73f <_init-0x691>
 6d2:	00 67 65             	add    %ah,0x65(%rdi)
 6d5:	74 68                	je     73f <_init-0x691>
 6d7:	6f                   	outsl  %ds:(%rsi),(%dx)
 6d8:	73 74                	jae    74e <_init-0x682>
 6da:	62                   	(bad)  
 6db:	79 6e                	jns    74b <_init-0x685>
 6dd:	61                   	(bad)  
 6de:	6d                   	insl   (%dx),%es:(%rdi)
 6df:	65 00 63 6c          	add    %ah,%gs:0x6c(%rbx)
 6e3:	6f                   	outsl  %ds:(%rsi),(%dx)
 6e4:	73 65                	jae    74b <_init-0x685>
 6e6:	00 73 6c             	add    %dh,0x6c(%rbx)
 6e9:	65 65 70 00          	gs gs jo 6ed <_init-0x6e3>
 6ed:	5f                   	pop    %rdi
 6ee:	5f                   	pop    %rdi
 6ef:	63 78 61             	movslq 0x61(%rax),%edi
 6f2:	5f                   	pop    %rdi
 6f3:	66 69 6e 61 6c 69    	imul   $0x696c,0x61(%rsi),%bp
 6f9:	7a 65                	jp     760 <_init-0x670>
 6fb:	00 5f 5f             	add    %bl,0x5f(%rdi)
 6fe:	73 70                	jae    770 <_init-0x660>
 700:	72 69                	jb     76b <_init-0x665>
 702:	6e                   	outsb  %ds:(%rsi),(%dx)
 703:	74 66                	je     76b <_init-0x665>
 705:	5f                   	pop    %rdi
 706:	63 68 6b             	movslq 0x6b(%rax),%ebp
 709:	00 5f 5f             	add    %bl,0x5f(%rdi)
 70c:	6c                   	insb   (%dx),%es:(%rdi)
 70d:	69 62 63 5f 73 74 61 	imul   $0x6174735f,0x63(%rdx),%esp
 714:	72 74                	jb     78a <_init-0x646>
 716:	5f                   	pop    %rdi
 717:	6d                   	insl   (%dx),%es:(%rdi)
 718:	61                   	(bad)  
 719:	69 6e 00 77 72 69 74 	imul   $0x74697277,0x0(%rsi),%ebp
 720:	65 00 47 4c          	add    %al,%gs:0x4c(%rdi)
 724:	49                   	rex.WB
 725:	42                   	rex.X
 726:	43 5f                	rex.XB pop %r15
 728:	32 2e                	xor    (%rsi),%ch
 72a:	33 00                	xor    (%rax),%eax
 72c:	47                   	rex.RXB
 72d:	4c                   	rex.WR
 72e:	49                   	rex.WB
 72f:	42                   	rex.X
 730:	43 5f                	rex.XB pop %r15
 732:	32 2e                	xor    (%rsi),%ch
 734:	37                   	(bad)  
 735:	00 47 4c             	add    %al,0x4c(%rdi)
 738:	49                   	rex.WB
 739:	42                   	rex.X
 73a:	43 5f                	rex.XB pop %r15
 73c:	32 2e                	xor    (%rsi),%ch
 73e:	33 2e                	xor    (%rsi),%ebp
 740:	34 00                	xor    $0x0,%al
 742:	47                   	rex.RXB
 743:	4c                   	rex.WR
 744:	49                   	rex.WB
 745:	42                   	rex.X
 746:	43 5f                	rex.XB pop %r15
 748:	32 2e                	xor    (%rsi),%ch
 74a:	34 00                	xor    $0x0,%al
 74c:	47                   	rex.RXB
 74d:	4c                   	rex.WR
 74e:	49                   	rex.WB
 74f:	42                   	rex.X
 750:	43 5f                	rex.XB pop %r15
 752:	32 2e                	xor    (%rsi),%ch
 754:	32 2e                	xor    (%rsi),%ch
 756:	35 00 5f 49 54       	xor    $0x54495f00,%eax
 75b:	4d 5f                	rex.WRB pop %r15
 75d:	64 65 72 65          	fs gs jb 7c6 <_init-0x60a>
 761:	67 69 73 74 65 72 54 	imul   $0x4d547265,0x74(%ebx),%esi
 768:	4d 
 769:	43 6c                	rex.XB insb (%dx),%es:(%rdi)
 76b:	6f                   	outsl  %ds:(%rsi),(%dx)
 76c:	6e                   	outsb  %ds:(%rsi),(%dx)
 76d:	65 54                	gs push %rsp
 76f:	61                   	(bad)  
 770:	62                   	(bad)  
 771:	6c                   	insb   (%dx),%es:(%rdi)
 772:	65 00 5f 5f          	add    %bl,%gs:0x5f(%rdi)
 776:	67 6d                	insl   (%dx),%es:(%edi)
 778:	6f                   	outsl  %ds:(%rsi),(%dx)
 779:	6e                   	outsb  %ds:(%rsi),(%dx)
 77a:	5f                   	pop    %rdi
 77b:	73 74                	jae    7f1 <_init-0x5df>
 77d:	61                   	(bad)  
 77e:	72 74                	jb     7f4 <_init-0x5dc>
 780:	5f                   	pop    %rdi
 781:	5f                   	pop    %rdi
 782:	00 5f 49             	add    %bl,0x49(%rdi)
 785:	54                   	push   %rsp
 786:	4d 5f                	rex.WRB pop %r15
 788:	72 65                	jb     7ef <_init-0x5e1>
 78a:	67 69 73 74 65 72 54 	imul   $0x4d547265,0x74(%ebx),%esi
 791:	4d 
 792:	43 6c                	rex.XB insb (%dx),%es:(%rdi)
 794:	6f                   	outsl  %ds:(%rsi),(%dx)
 795:	6e                   	outsb  %ds:(%rsi),(%dx)
 796:	65 54                	gs push %rsp
 798:	61                   	(bad)  
 799:	62                   	.byte 0x62
 79a:	6c                   	insb   (%dx),%es:(%rdi)
 79b:	65                   	gs
	...

Disassembly of section .gnu.version:

000000000000079e <.gnu.version>:
 79e:	00 00                	add    %al,(%rax)
 7a0:	02 00                	add    (%rax),%al
 7a2:	02 00                	add    (%rax),%al
 7a4:	00 00                	add    %al,(%rax)
 7a6:	02 00                	add    (%rax),%al
 7a8:	02 00                	add    (%rax),%al
 7aa:	02 00                	add    (%rax),%al
 7ac:	03 00                	add    (%rax),%eax
 7ae:	02 00                	add    (%rax),%al
 7b0:	02 00                	add    (%rax),%al
 7b2:	02 00                	add    (%rax),%al
 7b4:	02 00                	add    (%rax),%al
 7b6:	02 00                	add    (%rax),%al
 7b8:	02 00                	add    (%rax),%al
 7ba:	02 00                	add    (%rax),%al
 7bc:	04 00                	add    $0x0,%al
 7be:	00 00                	add    %al,(%rax)
 7c0:	02 00                	add    (%rax),%al
 7c2:	02 00                	add    (%rax),%al
 7c4:	05 00 04 00 02       	add    $0x2000400,%eax
 7c9:	00 02                	add    %al,(%rdx)
 7cb:	00 02                	add    %al,(%rdx)
 7cd:	00 04 00             	add    %al,(%rax,%rax,1)
 7d0:	00 00                	add    %al,(%rax)
 7d2:	02 00                	add    (%rax),%al
 7d4:	02 00                	add    (%rax),%al
 7d6:	06                   	(bad)  
 7d7:	00 04 00             	add    %al,(%rax,%rax,1)
 7da:	02 00                	add    (%rax),%al
 7dc:	02 00                	add    (%rax),%al
 7de:	02 00                	add    (%rax),%al
 7e0:	02 00                	add    (%rax),%al

Disassembly of section .gnu.version_r:

00000000000007e8 <.gnu.version_r>:
 7e8:	01 00                	add    %eax,(%rax)
 7ea:	05 00 01 00 00       	add    $0x100,%eax
 7ef:	00 10                	add    %dl,(%rax)
 7f1:	00 00                	add    %al,(%rax)
 7f3:	00 00                	add    %al,(%rax)
 7f5:	00 00                	add    %al,(%rax)
 7f7:	00 13                	add    %dl,(%rbx)
 7f9:	69 69 0d 00 00 06 00 	imul   $0x60000,0xd(%rcx),%ebp
 800:	2a 01                	sub    (%rcx),%al
 802:	00 00                	add    %al,(%rax)
 804:	10 00                	adc    %al,(%rax)
 806:	00 00                	add    %al,(%rax)
 808:	17                   	(bad)  
 809:	69 69 0d 00 00 05 00 	imul   $0x50000,0xd(%rcx),%ebp
 810:	34 01                	xor    $0x1,%al
 812:	00 00                	add    %al,(%rax)
 814:	10 00                	adc    %al,(%rax)
 816:	00 00                	add    %al,(%rax)
 818:	74 19                	je     833 <_init-0x59d>
 81a:	69 09 00 00 04 00    	imul   $0x40000,(%rcx),%ecx
 820:	3e 01 00             	add    %eax,%ds:(%rax)
 823:	00 10                	add    %dl,(%rax)
 825:	00 00                	add    %al,(%rax)
 827:	00 14 69             	add    %dl,(%rcx,%rbp,2)
 82a:	69 0d 00 00 03 00 4a 	imul   $0x14a,0x30000(%rip),%ecx        # 30834 <__FRAME_END__+0x2d688>
 831:	01 00 00 
 834:	10 00                	adc    %al,(%rax)
 836:	00 00                	add    %al,(%rax)
 838:	75 1a                	jne    854 <_init-0x57c>
 83a:	69 09 00 00 02 00    	imul   $0x20000,(%rcx),%ecx
 840:	54                   	push   %rsp
 841:	01 00                	add    %eax,(%rax)
 843:	00 00                	add    %al,(%rax)
 845:	00 00                	add    %al,(%rax)
	...

Disassembly of section .rela.dyn:

0000000000000848 <.rela.dyn>:
 848:	f8                   	clc    
 849:	3c 20                	cmp    $0x20,%al
 84b:	00 00                	add    %al,(%rax)
 84d:	00 00                	add    %al,(%rax)
 84f:	00 08                	add    %cl,(%rax)
 851:	00 00                	add    %al,(%rax)
 853:	00 00                	add    %al,(%rax)
 855:	00 00                	add    %al,(%rax)
 857:	00 a0 10 00 00 00    	add    %ah,0x10(%rax)
 85d:	00 00                	add    %al,(%rax)
 85f:	00 00                	add    %al,(%rax)
 861:	3d 20 00 00 00       	cmp    $0x20,%eax
 866:	00 00                	add    %al,(%rax)
 868:	08 00                	or     %al,(%rax)
 86a:	00 00                	add    %al,(%rax)
 86c:	00 00                	add    %al,(%rax)
 86e:	00 00                	add    %al,(%rax)
 870:	60                   	(bad)  
 871:	10 00                	adc    %al,(%rax)
 873:	00 00                	add    %al,(%rax)
 875:	00 00                	add    %al,(%rax)
 877:	00 08                	add    %cl,(%rax)
 879:	40 20 00             	and    %al,(%rax)
 87c:	00 00                	add    %al,(%rax)
 87e:	00 00                	add    %al,(%rax)
 880:	08 00                	or     %al,(%rax)
 882:	00 00                	add    %al,(%rax)
 884:	00 00                	add    %al,(%rax)
 886:	00 00                	add    %al,(%rax)
 888:	08 40 20             	or     %al,0x20(%rax)
 88b:	00 00                	add    %al,(%rax)
 88d:	00 00                	add    %al,(%rax)
 88f:	00 28                	add    %ch,(%rax)
 891:	41 20 00             	and    %al,(%r8)
 894:	00 00                	add    %al,(%rax)
 896:	00 00                	add    %al,(%rax)
 898:	08 00                	or     %al,(%rax)
 89a:	00 00                	add    %al,(%rax)
 89c:	00 00                	add    %al,(%rax)
 89e:	00 00                	add    %al,(%rax)
 8a0:	40                   	rex
 8a1:	41 20 00             	and    %al,(%r8)
 8a4:	00 00                	add    %al,(%rax)
 8a6:	00 00                	add    %al,(%rax)
 8a8:	30 41 20             	xor    %al,0x20(%rcx)
 8ab:	00 00                	add    %al,(%rax)
 8ad:	00 00                	add    %al,(%rax)
 8af:	00 08                	add    %cl,(%rax)
 8b1:	00 00                	add    %al,(%rax)
 8b3:	00 00                	add    %al,(%rax)
 8b5:	00 00                	add    %al,(%rax)
 8b7:	00 60 41             	add    %ah,0x41(%rax)
 8ba:	20 00                	and    %al,(%rax)
 8bc:	00 00                	add    %al,(%rax)
 8be:	00 00                	add    %al,(%rax)
 8c0:	48                   	rex.W
 8c1:	41 20 00             	and    %al,(%r8)
 8c4:	00 00                	add    %al,(%rax)
 8c6:	00 00                	add    %al,(%rax)
 8c8:	08 00                	or     %al,(%rax)
 8ca:	00 00                	add    %al,(%rax)
 8cc:	00 00                	add    %al,(%rax)
 8ce:	00 00                	add    %al,(%rax)
 8d0:	c0 41 20 00          	rolb   $0x0,0x20(%rcx)
 8d4:	00 00                	add    %al,(%rax)
 8d6:	00 00                	add    %al,(%rax)
 8d8:	50                   	push   %rax
 8d9:	41 20 00             	and    %al,(%r8)
 8dc:	00 00                	add    %al,(%rax)
 8de:	00 00                	add    %al,(%rax)
 8e0:	08 00                	or     %al,(%rax)
 8e2:	00 00                	add    %al,(%rax)
 8e4:	00 00                	add    %al,(%rax)
 8e6:	00 00                	add    %al,(%rax)
 8e8:	80 41 20 00          	addb   $0x0,0x20(%rcx)
 8ec:	00 00                	add    %al,(%rax)
 8ee:	00 00                	add    %al,(%rax)
 8f0:	68 41 20 00 00       	pushq  $0x2041
 8f5:	00 00                	add    %al,(%rax)
 8f7:	00 08                	add    %cl,(%rax)
 8f9:	00 00                	add    %al,(%rax)
 8fb:	00 00                	add    %al,(%rax)
 8fd:	00 00                	add    %al,(%rax)
 8ff:	00 a0 41 20 00 00    	add    %ah,0x2041(%rax)
 905:	00 00                	add    %al,(%rax)
 907:	00 70 41             	add    %dh,0x41(%rax)
 90a:	20 00                	and    %al,(%rax)
 90c:	00 00                	add    %al,(%rax)
 90e:	00 00                	add    %al,(%rax)
 910:	08 00                	or     %al,(%rax)
 912:	00 00                	add    %al,(%rax)
 914:	00 00                	add    %al,(%rax)
 916:	00 00                	add    %al,(%rax)
 918:	e0 41                	loopne 95b <_init-0x475>
 91a:	20 00                	and    %al,(%rax)
 91c:	00 00                	add    %al,(%rax)
 91e:	00 00                	add    %al,(%rax)
 920:	88 41 20             	mov    %al,0x20(%rcx)
 923:	00 00                	add    %al,(%rax)
 925:	00 00                	add    %al,(%rax)
 927:	00 08                	add    %cl,(%rax)
 929:	00 00                	add    %al,(%rax)
 92b:	00 00                	add    %al,(%rax)
 92d:	00 00                	add    %al,(%rax)
 92f:	00 b0 40 20 00 00    	add    %dh,0x2040(%rax)
 935:	00 00                	add    %al,(%rax)
 937:	00 90 41 20 00 00    	add    %dl,0x2041(%rax)
 93d:	00 00                	add    %al,(%rax)
 93f:	00 08                	add    %cl,(%rax)
 941:	00 00                	add    %al,(%rax)
 943:	00 00                	add    %al,(%rax)
 945:	00 00                	add    %al,(%rax)
 947:	00 70 40             	add    %dh,0x40(%rax)
 94a:	20 00                	and    %al,(%rax)
 94c:	00 00                	add    %al,(%rax)
 94e:	00 00                	add    %al,(%rax)
 950:	a8 41                	test   $0x41,%al
 952:	20 00                	and    %al,(%rax)
 954:	00 00                	add    %al,(%rax)
 956:	00 00                	add    %al,(%rax)
 958:	08 00                	or     %al,(%rax)
 95a:	00 00                	add    %al,(%rax)
 95c:	00 00                	add    %al,(%rax)
 95e:	00 00                	add    %al,(%rax)
 960:	10 40 20             	adc    %al,0x20(%rax)
 963:	00 00                	add    %al,(%rax)
 965:	00 00                	add    %al,(%rax)
 967:	00 b0 41 20 00 00    	add    %dh,0x2041(%rax)
 96d:	00 00                	add    %al,(%rax)
 96f:	00 08                	add    %cl,(%rax)
 971:	00 00                	add    %al,(%rax)
 973:	00 00                	add    %al,(%rax)
 975:	00 00                	add    %al,(%rax)
 977:	00 d0                	add    %dl,%al
 979:	40 20 00             	and    %al,(%rax)
 97c:	00 00                	add    %al,(%rax)
 97e:	00 00                	add    %al,(%rax)
 980:	c8 41 20 00          	enterq $0x2041,$0x0
 984:	00 00                	add    %al,(%rax)
 986:	00 00                	add    %al,(%rax)
 988:	08 00                	or     %al,(%rax)
 98a:	00 00                	add    %al,(%rax)
 98c:	00 00                	add    %al,(%rax)
 98e:	00 00                	add    %al,(%rax)
 990:	30 40 20             	xor    %al,0x20(%rax)
 993:	00 00                	add    %al,(%rax)
 995:	00 00                	add    %al,(%rax)
 997:	00 d0                	add    %dl,%al
 999:	41 20 00             	and    %al,(%r8)
 99c:	00 00                	add    %al,(%rax)
 99e:	00 00                	add    %al,(%rax)
 9a0:	08 00                	or     %al,(%rax)
 9a2:	00 00                	add    %al,(%rax)
 9a4:	00 00                	add    %al,(%rax)
 9a6:	00 00                	add    %al,(%rax)
 9a8:	90                   	nop
 9a9:	40 20 00             	and    %al,(%rax)
 9ac:	00 00                	add    %al,(%rax)
 9ae:	00 00                	add    %al,(%rax)
 9b0:	e8 41 20 00 00       	callq  29f6 <array.3413+0x176>
 9b5:	00 00                	add    %al,(%rax)
 9b7:	00 08                	add    %cl,(%rax)
 9b9:	00 00                	add    %al,(%rax)
 9bb:	00 00                	add    %al,(%rax)
 9bd:	00 00                	add    %al,(%rax)
 9bf:	00 50 40             	add    %dl,0x40(%rax)
 9c2:	20 00                	and    %al,(%rax)
 9c4:	00 00                	add    %al,(%rax)
 9c6:	00 00                	add    %al,(%rax)
 9c8:	f0 41 20 00          	lock and %al,(%r8)
 9cc:	00 00                	add    %al,(%rax)
 9ce:	00 00                	add    %al,(%rax)
 9d0:	08 00                	or     %al,(%rax)
 9d2:	00 00                	add    %al,(%rax)
 9d4:	00 00                	add    %al,(%rax)
 9d6:	00 00                	add    %al,(%rax)
 9d8:	f0 40 20 00          	lock and %al,(%rax)
 9dc:	00 00                	add    %al,(%rax)
 9de:	00 00                	add    %al,(%rax)
 9e0:	08 42 20             	or     %al,0x20(%rdx)
 9e3:	00 00                	add    %al,(%rax)
 9e5:	00 00                	add    %al,(%rax)
 9e7:	00 08                	add    %cl,(%rax)
 9e9:	00 00                	add    %al,(%rax)
 9eb:	00 00                	add    %al,(%rax)
 9ed:	00 00                	add    %al,(%rax)
 9ef:	00 10                	add    %dl,(%rax)
 9f1:	42 20 00             	rex.X and %al,(%rax)
 9f4:	00 00                	add    %al,(%rax)
 9f6:	00 00                	add    %al,(%rax)
 9f8:	18 42 20             	sbb    %al,0x20(%rdx)
 9fb:	00 00                	add    %al,(%rax)
 9fd:	00 00                	add    %al,(%rax)
 9ff:	00 08                	add    %cl,(%rax)
 a01:	00 00                	add    %al,(%rax)
 a03:	00 00                	add    %al,(%rax)
 a05:	00 00                	add    %al,(%rax)
 a07:	00 20                	add    %ah,(%rax)
 a09:	42 20 00             	rex.X and %al,(%rax)
 a0c:	00 00                	add    %al,(%rax)
 a0e:	00 00                	add    %al,(%rax)
 a10:	28 42 20             	sub    %al,0x20(%rdx)
 a13:	00 00                	add    %al,(%rax)
 a15:	00 00                	add    %al,(%rax)
 a17:	00 08                	add    %cl,(%rax)
 a19:	00 00                	add    %al,(%rax)
 a1b:	00 00                	add    %al,(%rax)
 a1d:	00 00                	add    %al,(%rax)
 a1f:	00 30                	add    %dh,(%rax)
 a21:	42 20 00             	rex.X and %al,(%rax)
 a24:	00 00                	add    %al,(%rax)
 a26:	00 00                	add    %al,(%rax)
 a28:	38 42 20             	cmp    %al,0x20(%rdx)
 a2b:	00 00                	add    %al,(%rax)
 a2d:	00 00                	add    %al,(%rax)
 a2f:	00 08                	add    %cl,(%rax)
 a31:	00 00                	add    %al,(%rax)
 a33:	00 00                	add    %al,(%rax)
 a35:	00 00                	add    %al,(%rax)
 a37:	00 40 42             	add    %al,0x42(%rax)
 a3a:	20 00                	and    %al,(%rax)
 a3c:	00 00                	add    %al,(%rax)
 a3e:	00 00                	add    %al,(%rax)
 a40:	48                   	rex.W
 a41:	42 20 00             	rex.X and %al,(%rax)
 a44:	00 00                	add    %al,(%rax)
 a46:	00 00                	add    %al,(%rax)
 a48:	08 00                	or     %al,(%rax)
 a4a:	00 00                	add    %al,(%rax)
 a4c:	00 00                	add    %al,(%rax)
 a4e:	00 00                	add    %al,(%rax)
 a50:	10 41 20             	adc    %al,0x20(%rcx)
 a53:	00 00                	add    %al,(%rax)
 a55:	00 00                	add    %al,(%rax)
 a57:	00 60 42             	add    %ah,0x42(%rax)
 a5a:	20 00                	and    %al,(%rax)
 a5c:	00 00                	add    %al,(%rax)
 a5e:	00 00                	add    %al,(%rax)
 a60:	08 00                	or     %al,(%rax)
 a62:	00 00                	add    %al,(%rax)
 a64:	00 00                	add    %al,(%rax)
 a66:	00 00                	add    %al,(%rax)
 a68:	29 2a                	sub    %ebp,(%rdx)
 a6a:	00 00                	add    %al,(%rax)
 a6c:	00 00                	add    %al,(%rax)
 a6e:	00 00                	add    %al,(%rax)
 a70:	68 42 20 00 00       	pushq  $0x2042
 a75:	00 00                	add    %al,(%rax)
 a77:	00 08                	add    %cl,(%rax)
 a79:	00 00                	add    %al,(%rax)
 a7b:	00 00                	add    %al,(%rax)
 a7d:	00 00                	add    %al,(%rax)
 a7f:	00 43 2a             	add    %al,0x2a(%rbx)
 a82:	00 00                	add    %al,(%rax)
 a84:	00 00                	add    %al,(%rax)
 a86:	00 00                	add    %al,(%rax)
 a88:	70 42                	jo     acc <_init-0x304>
 a8a:	20 00                	and    %al,(%rax)
 a8c:	00 00                	add    %al,(%rax)
 a8e:	00 00                	add    %al,(%rax)
 a90:	08 00                	or     %al,(%rax)
 a92:	00 00                	add    %al,(%rax)
 a94:	00 00                	add    %al,(%rax)
 a96:	00 00                	add    %al,(%rax)
 a98:	5d                   	pop    %rbp
 a99:	2a 00                	sub    (%rax),%al
 a9b:	00 00                	add    %al,(%rax)
 a9d:	00 00                	add    %al,(%rax)
 a9f:	00 78 42             	add    %bh,0x42(%rax)
 aa2:	20 00                	and    %al,(%rax)
 aa4:	00 00                	add    %al,(%rax)
 aa6:	00 00                	add    %al,(%rax)
 aa8:	08 00                	or     %al,(%rax)
 aaa:	00 00                	add    %al,(%rax)
 aac:	00 00                	add    %al,(%rax)
 aae:	00 00                	add    %al,(%rax)
 ab0:	76 2a                	jbe    adc <_init-0x2f4>
 ab2:	00 00                	add    %al,(%rax)
 ab4:	00 00                	add    %al,(%rax)
 ab6:	00 00                	add    %al,(%rax)
 ab8:	d8 3f                	fdivrs (%rdi)
 aba:	20 00                	and    %al,(%rax)
 abc:	00 00                	add    %al,(%rax)
 abe:	00 00                	add    %al,(%rax)
 ac0:	06                   	(bad)  
 ac1:	00 00                	add    %al,(%rax)
 ac3:	00 03                	add    %al,(%rbx)
	...
 acd:	00 00                	add    %al,(%rax)
 acf:	00 e0                	add    %ah,%al
 ad1:	3f                   	(bad)  
 ad2:	20 00                	and    %al,(%rax)
 ad4:	00 00                	add    %al,(%rax)
 ad6:	00 00                	add    %al,(%rax)
 ad8:	06                   	(bad)  
 ad9:	00 00                	add    %al,(%rax)
 adb:	00 0b                	add    %cl,(%rbx)
	...
 ae5:	00 00                	add    %al,(%rax)
 ae7:	00 e8                	add    %ch,%al
 ae9:	3f                   	(bad)  
 aea:	20 00                	and    %al,(%rax)
 aec:	00 00                	add    %al,(%rax)
 aee:	00 00                	add    %al,(%rax)
 af0:	06                   	(bad)  
 af1:	00 00                	add    %al,(%rax)
 af3:	00 10                	add    %dl,(%rax)
	...
 afd:	00 00                	add    %al,(%rax)
 aff:	00 f0                	add    %dh,%al
 b01:	3f                   	(bad)  
 b02:	20 00                	and    %al,(%rax)
 b04:	00 00                	add    %al,(%rax)
 b06:	00 00                	add    %al,(%rax)
 b08:	06                   	(bad)  
 b09:	00 00                	add    %al,(%rax)
 b0b:	00 19                	add    %bl,(%rcx)
	...
 b15:	00 00                	add    %al,(%rax)
 b17:	00 f8                	add    %bh,%al
 b19:	3f                   	(bad)  
 b1a:	20 00                	and    %al,(%rax)
 b1c:	00 00                	add    %al,(%rax)
 b1e:	00 00                	add    %al,(%rax)
 b20:	06                   	(bad)  
 b21:	00 00                	add    %al,(%rax)
 b23:	00 1b                	add    %bl,(%rbx)
	...
 b2d:	00 00                	add    %al,(%rax)
 b2f:	00 60 46             	add    %ah,0x46(%rax)
 b32:	20 00                	and    %al,(%rax)
 b34:	00 00                	add    %al,(%rax)
 b36:	00 00                	add    %al,(%rax)
 b38:	05 00 00 00 1f       	add    $0x1f000000,%eax
	...
 b45:	00 00                	add    %al,(%rax)
 b47:	00 70 46             	add    %dh,0x46(%rax)
 b4a:	20 00                	and    %al,(%rax)
 b4c:	00 00                	add    %al,(%rax)
 b4e:	00 00                	add    %al,(%rax)
 b50:	05 00 00 00 20       	add    $0x20000000,%eax
	...
 b5d:	00 00                	add    %al,(%rax)
 b5f:	00 80 46 20 00 00    	add    %al,0x2046(%rax)
 b65:	00 00                	add    %al,(%rax)
 b67:	00 05 00 00 00 21    	add    %al,0x21000000(%rip)        # 21000b6d <_end+0x20dfbe3d>
	...

Disassembly of section .rela.plt:

0000000000000b78 <.rela.plt>:
 b78:	10 3f                	adc    %bh,(%rdi)
 b7a:	20 00                	and    %al,(%rax)
 b7c:	00 00                	add    %al,(%rax)
 b7e:	00 00                	add    %al,(%rax)
 b80:	07                   	(bad)  
 b81:	00 00                	add    %al,(%rax)
 b83:	00 01                	add    %al,(%rcx)
	...
 b8d:	00 00                	add    %al,(%rax)
 b8f:	00 18                	add    %bl,(%rax)
 b91:	3f                   	(bad)  
 b92:	20 00                	and    %al,(%rax)
 b94:	00 00                	add    %al,(%rax)
 b96:	00 00                	add    %al,(%rax)
 b98:	07                   	(bad)  
 b99:	00 00                	add    %al,(%rax)
 b9b:	00 02                	add    %al,(%rdx)
	...
 ba5:	00 00                	add    %al,(%rax)
 ba7:	00 20                	add    %ah,(%rax)
 ba9:	3f                   	(bad)  
 baa:	20 00                	and    %al,(%rax)
 bac:	00 00                	add    %al,(%rax)
 bae:	00 00                	add    %al,(%rax)
 bb0:	07                   	(bad)  
 bb1:	00 00                	add    %al,(%rax)
 bb3:	00 04 00             	add    %al,(%rax,%rax,1)
	...
 bbe:	00 00                	add    %al,(%rax)
 bc0:	28 3f                	sub    %bh,(%rdi)
 bc2:	20 00                	and    %al,(%rax)
 bc4:	00 00                	add    %al,(%rax)
 bc6:	00 00                	add    %al,(%rax)
 bc8:	07                   	(bad)  
 bc9:	00 00                	add    %al,(%rax)
 bcb:	00 05 00 00 00 00    	add    %al,0x0(%rip)        # bd1 <_init-0x1ff>
 bd1:	00 00                	add    %al,(%rax)
 bd3:	00 00                	add    %al,(%rax)
 bd5:	00 00                	add    %al,(%rax)
 bd7:	00 30                	add    %dh,(%rax)
 bd9:	3f                   	(bad)  
 bda:	20 00                	and    %al,(%rax)
 bdc:	00 00                	add    %al,(%rax)
 bde:	00 00                	add    %al,(%rax)
 be0:	07                   	(bad)  
 be1:	00 00                	add    %al,(%rax)
 be3:	00 06                	add    %al,(%rsi)
	...
 bed:	00 00                	add    %al,(%rax)
 bef:	00 38                	add    %bh,(%rax)
 bf1:	3f                   	(bad)  
 bf2:	20 00                	and    %al,(%rax)
 bf4:	00 00                	add    %al,(%rax)
 bf6:	00 00                	add    %al,(%rax)
 bf8:	07                   	(bad)  
 bf9:	00 00                	add    %al,(%rax)
 bfb:	00 07                	add    %al,(%rdi)
	...
 c05:	00 00                	add    %al,(%rax)
 c07:	00 40 3f             	add    %al,0x3f(%rax)
 c0a:	20 00                	and    %al,(%rax)
 c0c:	00 00                	add    %al,(%rax)
 c0e:	00 00                	add    %al,(%rax)
 c10:	07                   	(bad)  
 c11:	00 00                	add    %al,(%rax)
 c13:	00 08                	add    %cl,(%rax)
	...
 c1d:	00 00                	add    %al,(%rax)
 c1f:	00 48 3f             	add    %cl,0x3f(%rax)
 c22:	20 00                	and    %al,(%rax)
 c24:	00 00                	add    %al,(%rax)
 c26:	00 00                	add    %al,(%rax)
 c28:	07                   	(bad)  
 c29:	00 00                	add    %al,(%rax)
 c2b:	00 09                	add    %cl,(%rcx)
	...
 c35:	00 00                	add    %al,(%rax)
 c37:	00 50 3f             	add    %dl,0x3f(%rax)
 c3a:	20 00                	and    %al,(%rax)
 c3c:	00 00                	add    %al,(%rax)
 c3e:	00 00                	add    %al,(%rax)
 c40:	07                   	(bad)  
 c41:	00 00                	add    %al,(%rax)
 c43:	00 0a                	add    %cl,(%rdx)
	...
 c4d:	00 00                	add    %al,(%rax)
 c4f:	00 58 3f             	add    %bl,0x3f(%rax)
 c52:	20 00                	and    %al,(%rax)
 c54:	00 00                	add    %al,(%rax)
 c56:	00 00                	add    %al,(%rax)
 c58:	07                   	(bad)  
 c59:	00 00                	add    %al,(%rax)
 c5b:	00 0c 00             	add    %cl,(%rax,%rax,1)
	...
 c66:	00 00                	add    %al,(%rax)
 c68:	60                   	(bad)  
 c69:	3f                   	(bad)  
 c6a:	20 00                	and    %al,(%rax)
 c6c:	00 00                	add    %al,(%rax)
 c6e:	00 00                	add    %al,(%rax)
 c70:	07                   	(bad)  
 c71:	00 00                	add    %al,(%rax)
 c73:	00 0d 00 00 00 00    	add    %cl,0x0(%rip)        # c79 <_init-0x157>
 c79:	00 00                	add    %al,(%rax)
 c7b:	00 00                	add    %al,(%rax)
 c7d:	00 00                	add    %al,(%rax)
 c7f:	00 68 3f             	add    %ch,0x3f(%rax)
 c82:	20 00                	and    %al,(%rax)
 c84:	00 00                	add    %al,(%rax)
 c86:	00 00                	add    %al,(%rax)
 c88:	07                   	(bad)  
 c89:	00 00                	add    %al,(%rax)
 c8b:	00 0e                	add    %cl,(%rsi)
	...
 c95:	00 00                	add    %al,(%rax)
 c97:	00 70 3f             	add    %dh,0x3f(%rax)
 c9a:	20 00                	and    %al,(%rax)
 c9c:	00 00                	add    %al,(%rax)
 c9e:	00 00                	add    %al,(%rax)
 ca0:	07                   	(bad)  
 ca1:	00 00                	add    %al,(%rax)
 ca3:	00 0f                	add    %cl,(%rdi)
	...
 cad:	00 00                	add    %al,(%rax)
 caf:	00 78 3f             	add    %bh,0x3f(%rax)
 cb2:	20 00                	and    %al,(%rax)
 cb4:	00 00                	add    %al,(%rax)
 cb6:	00 00                	add    %al,(%rax)
 cb8:	07                   	(bad)  
 cb9:	00 00                	add    %al,(%rax)
 cbb:	00 11                	add    %dl,(%rcx)
	...
 cc5:	00 00                	add    %al,(%rax)
 cc7:	00 80 3f 20 00 00    	add    %al,0x203f(%rax)
 ccd:	00 00                	add    %al,(%rax)
 ccf:	00 07                	add    %al,(%rdi)
 cd1:	00 00                	add    %al,(%rax)
 cd3:	00 12                	add    %dl,(%rdx)
	...
 cdd:	00 00                	add    %al,(%rax)
 cdf:	00 88 3f 20 00 00    	add    %cl,0x203f(%rax)
 ce5:	00 00                	add    %al,(%rax)
 ce7:	00 07                	add    %al,(%rdi)
 ce9:	00 00                	add    %al,(%rax)
 ceb:	00 13                	add    %dl,(%rbx)
	...
 cf5:	00 00                	add    %al,(%rax)
 cf7:	00 90 3f 20 00 00    	add    %dl,0x203f(%rax)
 cfd:	00 00                	add    %al,(%rax)
 cff:	00 07                	add    %al,(%rdi)
 d01:	00 00                	add    %al,(%rax)
 d03:	00 14 00             	add    %dl,(%rax,%rax,1)
	...
 d0e:	00 00                	add    %al,(%rax)
 d10:	98                   	cwtl   
 d11:	3f                   	(bad)  
 d12:	20 00                	and    %al,(%rax)
 d14:	00 00                	add    %al,(%rax)
 d16:	00 00                	add    %al,(%rax)
 d18:	07                   	(bad)  
 d19:	00 00                	add    %al,(%rax)
 d1b:	00 15 00 00 00 00    	add    %dl,0x0(%rip)        # d21 <_init-0xaf>
 d21:	00 00                	add    %al,(%rax)
 d23:	00 00                	add    %al,(%rax)
 d25:	00 00                	add    %al,(%rax)
 d27:	00 a0 3f 20 00 00    	add    %ah,0x203f(%rax)
 d2d:	00 00                	add    %al,(%rax)
 d2f:	00 07                	add    %al,(%rdi)
 d31:	00 00                	add    %al,(%rax)
 d33:	00 16                	add    %dl,(%rsi)
	...
 d3d:	00 00                	add    %al,(%rax)
 d3f:	00 a8 3f 20 00 00    	add    %ch,0x203f(%rax)
 d45:	00 00                	add    %al,(%rax)
 d47:	00 07                	add    %al,(%rdi)
 d49:	00 00                	add    %al,(%rax)
 d4b:	00 17                	add    %dl,(%rdi)
	...
 d55:	00 00                	add    %al,(%rax)
 d57:	00 b0 3f 20 00 00    	add    %dh,0x203f(%rax)
 d5d:	00 00                	add    %al,(%rax)
 d5f:	00 07                	add    %al,(%rdi)
 d61:	00 00                	add    %al,(%rax)
 d63:	00 18                	add    %bl,(%rax)
	...
 d6d:	00 00                	add    %al,(%rax)
 d6f:	00 b8 3f 20 00 00    	add    %bh,0x203f(%rax)
 d75:	00 00                	add    %al,(%rax)
 d77:	00 07                	add    %al,(%rdi)
 d79:	00 00                	add    %al,(%rax)
 d7b:	00 1a                	add    %bl,(%rdx)
	...
 d85:	00 00                	add    %al,(%rax)
 d87:	00 c0                	add    %al,%al
 d89:	3f                   	(bad)  
 d8a:	20 00                	and    %al,(%rax)
 d8c:	00 00                	add    %al,(%rax)
 d8e:	00 00                	add    %al,(%rax)
 d90:	07                   	(bad)  
 d91:	00 00                	add    %al,(%rax)
 d93:	00 1c 00             	add    %bl,(%rax,%rax,1)
	...
 d9e:	00 00                	add    %al,(%rax)
 da0:	c8 3f 20 00          	enterq $0x203f,$0x0
 da4:	00 00                	add    %al,(%rax)
 da6:	00 00                	add    %al,(%rax)
 da8:	07                   	(bad)  
 da9:	00 00                	add    %al,(%rax)
 dab:	00 1d 00 00 00 00    	add    %bl,0x0(%rip)        # db1 <_init-0x1f>
 db1:	00 00                	add    %al,(%rax)
 db3:	00 00                	add    %al,(%rax)
 db5:	00 00                	add    %al,(%rax)
 db7:	00 d0                	add    %dl,%al
 db9:	3f                   	(bad)  
 dba:	20 00                	and    %al,(%rax)
 dbc:	00 00                	add    %al,(%rax)
 dbe:	00 00                	add    %al,(%rax)
 dc0:	07                   	(bad)  
 dc1:	00 00                	add    %al,(%rax)
 dc3:	00 1e                	add    %bl,(%rsi)
	...

Disassembly of section .init:

0000000000000dd0 <_init>:
 dd0:	48 83 ec 08          	sub    $0x8,%rsp
 dd4:	48 8b 05 0d 32 20 00 	mov    0x20320d(%rip),%rax        # 203fe8 <__gmon_start__>
 ddb:	48 85 c0             	test   %rax,%rax
 dde:	74 02                	je     de2 <_init+0x12>
 de0:	ff d0                	callq  *%rax
 de2:	48 83 c4 08          	add    $0x8,%rsp
 de6:	c3                   	retq   

Disassembly of section .plt:

0000000000000df0 <.plt>:
 df0:	ff 35 0a 31 20 00    	pushq  0x20310a(%rip)        # 203f00 <_GLOBAL_OFFSET_TABLE_+0x8>
 df6:	ff 25 0c 31 20 00    	jmpq   *0x20310c(%rip)        # 203f08 <_GLOBAL_OFFSET_TABLE_+0x10>
 dfc:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000000e00 <getenv@plt>:
 e00:	ff 25 0a 31 20 00    	jmpq   *0x20310a(%rip)        # 203f10 <getenv@GLIBC_2.2.5>
 e06:	68 00 00 00 00       	pushq  $0x0
 e0b:	e9 e0 ff ff ff       	jmpq   df0 <.plt>

0000000000000e10 <__errno_location@plt>:
 e10:	ff 25 02 31 20 00    	jmpq   *0x203102(%rip)        # 203f18 <__errno_location@GLIBC_2.2.5>
 e16:	68 01 00 00 00       	pushq  $0x1
 e1b:	e9 d0 ff ff ff       	jmpq   df0 <.plt>

0000000000000e20 <strcpy@plt>:
 e20:	ff 25 fa 30 20 00    	jmpq   *0x2030fa(%rip)        # 203f20 <strcpy@GLIBC_2.2.5>
 e26:	68 02 00 00 00       	pushq  $0x2
 e2b:	e9 c0 ff ff ff       	jmpq   df0 <.plt>

0000000000000e30 <puts@plt>:
 e30:	ff 25 f2 30 20 00    	jmpq   *0x2030f2(%rip)        # 203f28 <puts@GLIBC_2.2.5>
 e36:	68 03 00 00 00       	pushq  $0x3
 e3b:	e9 b0 ff ff ff       	jmpq   df0 <.plt>

0000000000000e40 <write@plt>:
 e40:	ff 25 ea 30 20 00    	jmpq   *0x2030ea(%rip)        # 203f30 <write@GLIBC_2.2.5>
 e46:	68 04 00 00 00       	pushq  $0x4
 e4b:	e9 a0 ff ff ff       	jmpq   df0 <.plt>

0000000000000e50 <__stack_chk_fail@plt>:
 e50:	ff 25 e2 30 20 00    	jmpq   *0x2030e2(%rip)        # 203f38 <__stack_chk_fail@GLIBC_2.4>
 e56:	68 05 00 00 00       	pushq  $0x5
 e5b:	e9 90 ff ff ff       	jmpq   df0 <.plt>

0000000000000e60 <alarm@plt>:
 e60:	ff 25 da 30 20 00    	jmpq   *0x2030da(%rip)        # 203f40 <alarm@GLIBC_2.2.5>
 e66:	68 06 00 00 00       	pushq  $0x6
 e6b:	e9 80 ff ff ff       	jmpq   df0 <.plt>

0000000000000e70 <close@plt>:
 e70:	ff 25 d2 30 20 00    	jmpq   *0x2030d2(%rip)        # 203f48 <close@GLIBC_2.2.5>
 e76:	68 07 00 00 00       	pushq  $0x7
 e7b:	e9 70 ff ff ff       	jmpq   df0 <.plt>

0000000000000e80 <read@plt>:
 e80:	ff 25 ca 30 20 00    	jmpq   *0x2030ca(%rip)        # 203f50 <read@GLIBC_2.2.5>
 e86:	68 08 00 00 00       	pushq  $0x8
 e8b:	e9 60 ff ff ff       	jmpq   df0 <.plt>

0000000000000e90 <fgets@plt>:
 e90:	ff 25 c2 30 20 00    	jmpq   *0x2030c2(%rip)        # 203f58 <fgets@GLIBC_2.2.5>
 e96:	68 09 00 00 00       	pushq  $0x9
 e9b:	e9 50 ff ff ff       	jmpq   df0 <.plt>

0000000000000ea0 <signal@plt>:
 ea0:	ff 25 ba 30 20 00    	jmpq   *0x2030ba(%rip)        # 203f60 <signal@GLIBC_2.2.5>
 ea6:	68 0a 00 00 00       	pushq  $0xa
 eab:	e9 40 ff ff ff       	jmpq   df0 <.plt>

0000000000000eb0 <gethostbyname@plt>:
 eb0:	ff 25 b2 30 20 00    	jmpq   *0x2030b2(%rip)        # 203f68 <gethostbyname@GLIBC_2.2.5>
 eb6:	68 0b 00 00 00       	pushq  $0xb
 ebb:	e9 30 ff ff ff       	jmpq   df0 <.plt>

0000000000000ec0 <__memmove_chk@plt>:
 ec0:	ff 25 aa 30 20 00    	jmpq   *0x2030aa(%rip)        # 203f70 <__memmove_chk@GLIBC_2.3.4>
 ec6:	68 0c 00 00 00       	pushq  $0xc
 ecb:	e9 20 ff ff ff       	jmpq   df0 <.plt>

0000000000000ed0 <strtol@plt>:
 ed0:	ff 25 a2 30 20 00    	jmpq   *0x2030a2(%rip)        # 203f78 <strtol@GLIBC_2.2.5>
 ed6:	68 0d 00 00 00       	pushq  $0xd
 edb:	e9 10 ff ff ff       	jmpq   df0 <.plt>

0000000000000ee0 <fflush@plt>:
 ee0:	ff 25 9a 30 20 00    	jmpq   *0x20309a(%rip)        # 203f80 <fflush@GLIBC_2.2.5>
 ee6:	68 0e 00 00 00       	pushq  $0xe
 eeb:	e9 00 ff ff ff       	jmpq   df0 <.plt>

0000000000000ef0 <__isoc99_sscanf@plt>:
 ef0:	ff 25 92 30 20 00    	jmpq   *0x203092(%rip)        # 203f88 <__isoc99_sscanf@GLIBC_2.7>
 ef6:	68 0f 00 00 00       	pushq  $0xf
 efb:	e9 f0 fe ff ff       	jmpq   df0 <.plt>

0000000000000f00 <__printf_chk@plt>:
 f00:	ff 25 8a 30 20 00    	jmpq   *0x20308a(%rip)        # 203f90 <__printf_chk@GLIBC_2.3.4>
 f06:	68 10 00 00 00       	pushq  $0x10
 f0b:	e9 e0 fe ff ff       	jmpq   df0 <.plt>

0000000000000f10 <fopen@plt>:
 f10:	ff 25 82 30 20 00    	jmpq   *0x203082(%rip)        # 203f98 <fopen@GLIBC_2.2.5>
 f16:	68 11 00 00 00       	pushq  $0x11
 f1b:	e9 d0 fe ff ff       	jmpq   df0 <.plt>

0000000000000f20 <exit@plt>:
 f20:	ff 25 7a 30 20 00    	jmpq   *0x20307a(%rip)        # 203fa0 <exit@GLIBC_2.2.5>
 f26:	68 12 00 00 00       	pushq  $0x12
 f2b:	e9 c0 fe ff ff       	jmpq   df0 <.plt>

0000000000000f30 <connect@plt>:
 f30:	ff 25 72 30 20 00    	jmpq   *0x203072(%rip)        # 203fa8 <connect@GLIBC_2.2.5>
 f36:	68 13 00 00 00       	pushq  $0x13
 f3b:	e9 b0 fe ff ff       	jmpq   df0 <.plt>

0000000000000f40 <__fprintf_chk@plt>:
 f40:	ff 25 6a 30 20 00    	jmpq   *0x20306a(%rip)        # 203fb0 <__fprintf_chk@GLIBC_2.3.4>
 f46:	68 14 00 00 00       	pushq  $0x14
 f4b:	e9 a0 fe ff ff       	jmpq   df0 <.plt>

0000000000000f50 <sleep@plt>:
 f50:	ff 25 62 30 20 00    	jmpq   *0x203062(%rip)        # 203fb8 <sleep@GLIBC_2.2.5>
 f56:	68 15 00 00 00       	pushq  $0x15
 f5b:	e9 90 fe ff ff       	jmpq   df0 <.plt>

0000000000000f60 <__ctype_b_loc@plt>:
 f60:	ff 25 5a 30 20 00    	jmpq   *0x20305a(%rip)        # 203fc0 <__ctype_b_loc@GLIBC_2.3>
 f66:	68 16 00 00 00       	pushq  $0x16
 f6b:	e9 80 fe ff ff       	jmpq   df0 <.plt>

0000000000000f70 <__sprintf_chk@plt>:
 f70:	ff 25 52 30 20 00    	jmpq   *0x203052(%rip)        # 203fc8 <__sprintf_chk@GLIBC_2.3.4>
 f76:	68 17 00 00 00       	pushq  $0x17
 f7b:	e9 70 fe ff ff       	jmpq   df0 <.plt>

0000000000000f80 <socket@plt>:
 f80:	ff 25 4a 30 20 00    	jmpq   *0x20304a(%rip)        # 203fd0 <socket@GLIBC_2.2.5>
 f86:	68 18 00 00 00       	pushq  $0x18
 f8b:	e9 60 fe ff ff       	jmpq   df0 <.plt>

Disassembly of section .plt.got:

0000000000000f90 <__cxa_finalize@plt>:
 f90:	ff 25 62 30 20 00    	jmpq   *0x203062(%rip)        # 203ff8 <__cxa_finalize@GLIBC_2.2.5>
 f96:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

0000000000000fa0 <_start>:
     fa0:	31 ed                	xor    %ebp,%ebp
     fa2:	49 89 d1             	mov    %rdx,%r9
     fa5:	5e                   	pop    %rsi
     fa6:	48 89 e2             	mov    %rsp,%rdx
     fa9:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
     fad:	50                   	push   %rax
     fae:	54                   	push   %rsp
     faf:	4c 8d 05 ca 16 00 00 	lea    0x16ca(%rip),%r8        # 2680 <__libc_csu_fini>
     fb6:	48 8d 0d 53 16 00 00 	lea    0x1653(%rip),%rcx        # 2610 <__libc_csu_init>
     fbd:	48 8d 3d e6 00 00 00 	lea    0xe6(%rip),%rdi        # 10aa <main>
     fc4:	ff 15 16 30 20 00    	callq  *0x203016(%rip)        # 203fe0 <__libc_start_main@GLIBC_2.2.5>
     fca:	f4                   	hlt    
     fcb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000000fd0 <deregister_tm_clones>:
     fd0:	48 8d 3d 89 36 20 00 	lea    0x203689(%rip),%rdi        # 204660 <stdout@@GLIBC_2.2.5>
     fd7:	55                   	push   %rbp
     fd8:	48 8d 05 81 36 20 00 	lea    0x203681(%rip),%rax        # 204660 <stdout@@GLIBC_2.2.5>
     fdf:	48 39 f8             	cmp    %rdi,%rax
     fe2:	48 89 e5             	mov    %rsp,%rbp
     fe5:	74 19                	je     1000 <deregister_tm_clones+0x30>
     fe7:	48 8b 05 ea 2f 20 00 	mov    0x202fea(%rip),%rax        # 203fd8 <_ITM_deregisterTMCloneTable>
     fee:	48 85 c0             	test   %rax,%rax
     ff1:	74 0d                	je     1000 <deregister_tm_clones+0x30>
     ff3:	5d                   	pop    %rbp
     ff4:	ff e0                	jmpq   *%rax
     ff6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
     ffd:	00 00 00 
    1000:	5d                   	pop    %rbp
    1001:	c3                   	retq   
    1002:	0f 1f 40 00          	nopl   0x0(%rax)
    1006:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    100d:	00 00 00 

0000000000001010 <register_tm_clones>:
    1010:	48 8d 3d 49 36 20 00 	lea    0x203649(%rip),%rdi        # 204660 <stdout@@GLIBC_2.2.5>
    1017:	48 8d 35 42 36 20 00 	lea    0x203642(%rip),%rsi        # 204660 <stdout@@GLIBC_2.2.5>
    101e:	55                   	push   %rbp
    101f:	48 29 fe             	sub    %rdi,%rsi
    1022:	48 89 e5             	mov    %rsp,%rbp
    1025:	48 c1 fe 03          	sar    $0x3,%rsi
    1029:	48 89 f0             	mov    %rsi,%rax
    102c:	48 c1 e8 3f          	shr    $0x3f,%rax
    1030:	48 01 c6             	add    %rax,%rsi
    1033:	48 d1 fe             	sar    %rsi
    1036:	74 18                	je     1050 <register_tm_clones+0x40>
    1038:	48 8b 05 b1 2f 20 00 	mov    0x202fb1(%rip),%rax        # 203ff0 <_ITM_registerTMCloneTable>
    103f:	48 85 c0             	test   %rax,%rax
    1042:	74 0c                	je     1050 <register_tm_clones+0x40>
    1044:	5d                   	pop    %rbp
    1045:	ff e0                	jmpq   *%rax
    1047:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
    104e:	00 00 
    1050:	5d                   	pop    %rbp
    1051:	c3                   	retq   
    1052:	0f 1f 40 00          	nopl   0x0(%rax)
    1056:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    105d:	00 00 00 

0000000000001060 <__do_global_dtors_aux>:
    1060:	80 3d 21 36 20 00 00 	cmpb   $0x0,0x203621(%rip)        # 204688 <completed.7698>
    1067:	75 2f                	jne    1098 <__do_global_dtors_aux+0x38>
    1069:	48 83 3d 87 2f 20 00 	cmpq   $0x0,0x202f87(%rip)        # 203ff8 <__cxa_finalize@GLIBC_2.2.5>
    1070:	00 
    1071:	55                   	push   %rbp
    1072:	48 89 e5             	mov    %rsp,%rbp
    1075:	74 0c                	je     1083 <__do_global_dtors_aux+0x23>
    1077:	48 8b 3d 8a 2f 20 00 	mov    0x202f8a(%rip),%rdi        # 204008 <__dso_handle>
    107e:	e8 0d ff ff ff       	callq  f90 <__cxa_finalize@plt>
    1083:	e8 48 ff ff ff       	callq  fd0 <deregister_tm_clones>
    1088:	c6 05 f9 35 20 00 01 	movb   $0x1,0x2035f9(%rip)        # 204688 <completed.7698>
    108f:	5d                   	pop    %rbp
    1090:	c3                   	retq   
    1091:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    1098:	f3 c3                	repz retq 
    109a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000000010a0 <frame_dummy>:
    10a0:	55                   	push   %rbp
    10a1:	48 89 e5             	mov    %rsp,%rbp
    10a4:	5d                   	pop    %rbp
    10a5:	e9 66 ff ff ff       	jmpq   1010 <register_tm_clones>

00000000000010aa <main>:
    10aa:	53                   	push   %rbx
    10ab:	83 ff 01             	cmp    $0x1,%edi
    10ae:	0f 84 f8 00 00 00    	je     11ac <main+0x102>
    10b4:	48 89 f3             	mov    %rsi,%rbx
    10b7:	83 ff 02             	cmp    $0x2,%edi
    10ba:	0f 85 21 01 00 00    	jne    11e1 <main+0x137>
    10c0:	48 8b 7e 08          	mov    0x8(%rsi),%rdi
    10c4:	48 8d 35 d9 15 00 00 	lea    0x15d9(%rip),%rsi        # 26a4 <_IO_stdin_used+0x4>
    10cb:	e8 40 fe ff ff       	callq  f10 <fopen@plt>
    10d0:	48 89 05 c1 35 20 00 	mov    %rax,0x2035c1(%rip)        # 204698 <infile>
    10d7:	48 85 c0             	test   %rax,%rax
    10da:	0f 84 df 00 00 00    	je     11bf <main+0x115>
    10e0:	e8 5d 06 00 00       	callq  1742 <initialize_bomb>
    10e5:	48 8d 3d 3c 16 00 00 	lea    0x163c(%rip),%rdi        # 2728 <_IO_stdin_used+0x88>
    10ec:	e8 3f fd ff ff       	callq  e30 <puts@plt>
    10f1:	48 8d 3d 70 16 00 00 	lea    0x1670(%rip),%rdi        # 2768 <_IO_stdin_used+0xc8>
    10f8:	e8 33 fd ff ff       	callq  e30 <puts@plt>
    10fd:	e8 4c 07 00 00       	callq  184e <read_line>
    1102:	48 89 c7             	mov    %rax,%rdi
    1105:	e8 fa 00 00 00       	callq  1204 <phase_1>
    110a:	e8 83 08 00 00       	callq  1992 <phase_defused>
    110f:	48 8d 3d 82 16 00 00 	lea    0x1682(%rip),%rdi        # 2798 <_IO_stdin_used+0xf8>
    1116:	e8 15 fd ff ff       	callq  e30 <puts@plt>
    111b:	e8 2e 07 00 00       	callq  184e <read_line>
    1120:	48 89 c7             	mov    %rax,%rdi
    1123:	e8 fc 00 00 00       	callq  1224 <phase_2>
    1128:	e8 65 08 00 00       	callq  1992 <phase_defused>
    112d:	48 8d 3d a9 15 00 00 	lea    0x15a9(%rip),%rdi        # 26dd <_IO_stdin_used+0x3d>
    1134:	e8 f7 fc ff ff       	callq  e30 <puts@plt>
    1139:	e8 10 07 00 00       	callq  184e <read_line>
    113e:	48 89 c7             	mov    %rax,%rdi
    1141:	e8 4d 01 00 00       	callq  1293 <phase_3>
    1146:	e8 47 08 00 00       	callq  1992 <phase_defused>
    114b:	48 8d 3d a9 15 00 00 	lea    0x15a9(%rip),%rdi        # 26fb <_IO_stdin_used+0x5b>
    1152:	e8 d9 fc ff ff       	callq  e30 <puts@plt>
    1157:	e8 f2 06 00 00       	callq  184e <read_line>
    115c:	48 89 c7             	mov    %rax,%rdi
    115f:	e8 1c 02 00 00       	callq  1380 <phase_4>
    1164:	e8 29 08 00 00       	callq  1992 <phase_defused>
    1169:	48 8d 3d 58 16 00 00 	lea    0x1658(%rip),%rdi        # 27c8 <_IO_stdin_used+0x128>
    1170:	e8 bb fc ff ff       	callq  e30 <puts@plt>
    1175:	e8 d4 06 00 00       	callq  184e <read_line>
    117a:	48 89 c7             	mov    %rax,%rdi
    117d:	e8 6d 02 00 00       	callq  13ef <phase_5>
    1182:	e8 0b 08 00 00       	callq  1992 <phase_defused>
    1187:	48 8d 3d 7c 15 00 00 	lea    0x157c(%rip),%rdi        # 270a <_IO_stdin_used+0x6a>
    118e:	e8 9d fc ff ff       	callq  e30 <puts@plt>
    1193:	e8 b6 06 00 00       	callq  184e <read_line>
    1198:	48 89 c7             	mov    %rax,%rdi
    119b:	e8 e2 02 00 00       	callq  1482 <phase_6>
    11a0:	e8 ed 07 00 00       	callq  1992 <phase_defused>
    11a5:	b8 00 00 00 00       	mov    $0x0,%eax
    11aa:	5b                   	pop    %rbx
    11ab:	c3                   	retq   
    11ac:	48 8b 05 bd 34 20 00 	mov    0x2034bd(%rip),%rax        # 204670 <stdin@@GLIBC_2.2.5>
    11b3:	48 89 05 de 34 20 00 	mov    %rax,0x2034de(%rip)        # 204698 <infile>
    11ba:	e9 21 ff ff ff       	jmpq   10e0 <main+0x36>
    11bf:	48 8b 4b 08          	mov    0x8(%rbx),%rcx
    11c3:	48 8b 13             	mov    (%rbx),%rdx
    11c6:	48 8d 35 d9 14 00 00 	lea    0x14d9(%rip),%rsi        # 26a6 <_IO_stdin_used+0x6>
    11cd:	bf 01 00 00 00       	mov    $0x1,%edi
    11d2:	e8 29 fd ff ff       	callq  f00 <__printf_chk@plt>
    11d7:	bf 08 00 00 00       	mov    $0x8,%edi
    11dc:	e8 3f fd ff ff       	callq  f20 <exit@plt>
    11e1:	48 8b 16             	mov    (%rsi),%rdx
    11e4:	48 8d 35 d8 14 00 00 	lea    0x14d8(%rip),%rsi        # 26c3 <_IO_stdin_used+0x23>
    11eb:	bf 01 00 00 00       	mov    $0x1,%edi
    11f0:	b8 00 00 00 00       	mov    $0x0,%eax
    11f5:	e8 06 fd ff ff       	callq  f00 <__printf_chk@plt>
    11fa:	bf 08 00 00 00       	mov    $0x8,%edi
    11ff:	e8 1c fd ff ff       	callq  f20 <exit@plt>

0000000000001204 <phase_1>:
    1204:	48 83 ec 08          	sub    $0x8,%rsp
    1208:	48 8d 35 e1 15 00 00 	lea    0x15e1(%rip),%rsi        # 27f0 <_IO_stdin_used+0x150>
    120f:	e8 c7 04 00 00       	callq  16db <strings_not_equal>
    1214:	85 c0                	test   %eax,%eax
    1216:	75 05                	jne    121d <phase_1+0x19>
    1218:	48 83 c4 08          	add    $0x8,%rsp
    121c:	c3                   	retq   
    121d:	e8 c5 05 00 00       	callq  17e7 <explode_bomb>
    1222:	eb f4                	jmp    1218 <phase_1+0x14>

0000000000001224 <phase_2>:
    1224:	55                   	push   %rbp
    1225:	53                   	push   %rbx
    1226:	48 83 ec 28          	sub    $0x28,%rsp
    122a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1231:	00 00 
    1233:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
    1238:	31 c0                	xor    %eax,%eax
    123a:	48 89 e6             	mov    %rsp,%rsi
    123d:	e8 cb 05 00 00       	callq  180d <read_six_numbers>
    1242:	83 3c 24 00          	cmpl   $0x0,(%rsp)
    1246:	75 07                	jne    124f <phase_2+0x2b>
    1248:	83 7c 24 04 01       	cmpl   $0x1,0x4(%rsp)
    124d:	74 05                	je     1254 <phase_2+0x30>
    124f:	e8 93 05 00 00       	callq  17e7 <explode_bomb>
    1254:	48 89 e3             	mov    %rsp,%rbx
    1257:	48 8d 6b 10          	lea    0x10(%rbx),%rbp
    125b:	eb 09                	jmp    1266 <phase_2+0x42>
    125d:	48 83 c3 04          	add    $0x4,%rbx
    1261:	48 39 eb             	cmp    %rbp,%rbx
    1264:	74 11                	je     1277 <phase_2+0x53>
    1266:	8b 43 04             	mov    0x4(%rbx),%eax
    1269:	03 03                	add    (%rbx),%eax
    126b:	39 43 08             	cmp    %eax,0x8(%rbx)
    126e:	74 ed                	je     125d <phase_2+0x39>
    1270:	e8 72 05 00 00       	callq  17e7 <explode_bomb>
    1275:	eb e6                	jmp    125d <phase_2+0x39>
    1277:	48 8b 44 24 18       	mov    0x18(%rsp),%rax
    127c:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    1283:	00 00 
    1285:	75 07                	jne    128e <phase_2+0x6a>
    1287:	48 83 c4 28          	add    $0x28,%rsp
    128b:	5b                   	pop    %rbx
    128c:	5d                   	pop    %rbp
    128d:	c3                   	retq   
    128e:	e8 bd fb ff ff       	callq  e50 <__stack_chk_fail@plt>

0000000000001293 <phase_3>:
    1293:	48 83 ec 18          	sub    $0x18,%rsp
    1297:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    129e:	00 00 
    12a0:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
    12a5:	31 c0                	xor    %eax,%eax
    12a7:	48 8d 4c 24 04       	lea    0x4(%rsp),%rcx
    12ac:	48 89 e2             	mov    %rsp,%rdx
    12af:	48 8d 35 19 17 00 00 	lea    0x1719(%rip),%rsi        # 29cf <array.3413+0x14f>
    12b6:	e8 35 fc ff ff       	callq  ef0 <__isoc99_sscanf@plt>
    12bb:	83 f8 01             	cmp    $0x1,%eax
    12be:	7e 19                	jle    12d9 <phase_3+0x46>
    12c0:	83 3c 24 07          	cmpl   $0x7,(%rsp)
    12c4:	77 4b                	ja     1311 <phase_3+0x7e>
    12c6:	8b 04 24             	mov    (%rsp),%eax
    12c9:	48 8d 15 90 15 00 00 	lea    0x1590(%rip),%rdx        # 2860 <_IO_stdin_used+0x1c0>
    12d0:	48 63 04 82          	movslq (%rdx,%rax,4),%rax
    12d4:	48 01 d0             	add    %rdx,%rax
    12d7:	ff e0                	jmpq   *%rax
    12d9:	e8 09 05 00 00       	callq  17e7 <explode_bomb>
    12de:	eb e0                	jmp    12c0 <phase_3+0x2d>
    12e0:	b8 39 03 00 00       	mov    $0x339,%eax
    12e5:	eb 3b                	jmp    1322 <phase_3+0x8f>
    12e7:	b8 3c 01 00 00       	mov    $0x13c,%eax
    12ec:	eb 34                	jmp    1322 <phase_3+0x8f>
    12ee:	b8 61 02 00 00       	mov    $0x261,%eax
    12f3:	eb 2d                	jmp    1322 <phase_3+0x8f>
    12f5:	b8 4a 03 00 00       	mov    $0x34a,%eax
    12fa:	eb 26                	jmp    1322 <phase_3+0x8f>
    12fc:	b8 c1 00 00 00       	mov    $0xc1,%eax
    1301:	eb 1f                	jmp    1322 <phase_3+0x8f>
    1303:	b8 11 03 00 00       	mov    $0x311,%eax
    1308:	eb 18                	jmp    1322 <phase_3+0x8f>
    130a:	b8 6d 03 00 00       	mov    $0x36d,%eax
    130f:	eb 11                	jmp    1322 <phase_3+0x8f>
    1311:	e8 d1 04 00 00       	callq  17e7 <explode_bomb>
    1316:	b8 00 00 00 00       	mov    $0x0,%eax
    131b:	eb 05                	jmp    1322 <phase_3+0x8f>
    131d:	b8 48 02 00 00       	mov    $0x248,%eax
    1322:	39 44 24 04          	cmp    %eax,0x4(%rsp)
    1326:	74 05                	je     132d <phase_3+0x9a>
    1328:	e8 ba 04 00 00       	callq  17e7 <explode_bomb>
    132d:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
    1332:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    1339:	00 00 
    133b:	75 05                	jne    1342 <phase_3+0xaf>
    133d:	48 83 c4 18          	add    $0x18,%rsp
    1341:	c3                   	retq   
    1342:	e8 09 fb ff ff       	callq  e50 <__stack_chk_fail@plt>

0000000000001347 <func4>:
    1347:	b8 00 00 00 00       	mov    $0x0,%eax
    134c:	85 ff                	test   %edi,%edi
    134e:	7e 07                	jle    1357 <func4+0x10>
    1350:	89 f0                	mov    %esi,%eax
    1352:	83 ff 01             	cmp    $0x1,%edi
    1355:	75 02                	jne    1359 <func4+0x12>
    1357:	f3 c3                	repz retq 
    1359:	41 54                	push   %r12
    135b:	55                   	push   %rbp
    135c:	53                   	push   %rbx
    135d:	41 89 f4             	mov    %esi,%r12d
    1360:	89 fb                	mov    %edi,%ebx
    1362:	8d 7f ff             	lea    -0x1(%rdi),%edi
    1365:	e8 dd ff ff ff       	callq  1347 <func4>
    136a:	42 8d 2c 20          	lea    (%rax,%r12,1),%ebp
    136e:	8d 7b fe             	lea    -0x2(%rbx),%edi
    1371:	44 89 e6             	mov    %r12d,%esi
    1374:	e8 ce ff ff ff       	callq  1347 <func4>
    1379:	01 e8                	add    %ebp,%eax
    137b:	5b                   	pop    %rbx
    137c:	5d                   	pop    %rbp
    137d:	41 5c                	pop    %r12
    137f:	c3                   	retq   

0000000000001380 <phase_4>:
    1380:	48 83 ec 18          	sub    $0x18,%rsp
    1384:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    138b:	00 00 
    138d:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
    1392:	31 c0                	xor    %eax,%eax
    1394:	48 89 e1             	mov    %rsp,%rcx
    1397:	48 8d 54 24 04       	lea    0x4(%rsp),%rdx
    139c:	48 8d 35 2c 16 00 00 	lea    0x162c(%rip),%rsi        # 29cf <array.3413+0x14f>
    13a3:	e8 48 fb ff ff       	callq  ef0 <__isoc99_sscanf@plt>
    13a8:	83 f8 02             	cmp    $0x2,%eax
    13ab:	75 0b                	jne    13b8 <phase_4+0x38>
    13ad:	8b 04 24             	mov    (%rsp),%eax
    13b0:	83 e8 02             	sub    $0x2,%eax
    13b3:	83 f8 02             	cmp    $0x2,%eax
    13b6:	76 05                	jbe    13bd <phase_4+0x3d>
    13b8:	e8 2a 04 00 00       	callq  17e7 <explode_bomb>
    13bd:	8b 34 24             	mov    (%rsp),%esi
    13c0:	bf 05 00 00 00       	mov    $0x5,%edi
    13c5:	e8 7d ff ff ff       	callq  1347 <func4>
    13ca:	39 44 24 04          	cmp    %eax,0x4(%rsp)
    13ce:	74 05                	je     13d5 <phase_4+0x55>
    13d0:	e8 12 04 00 00       	callq  17e7 <explode_bomb>
    13d5:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
    13da:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    13e1:	00 00 
    13e3:	75 05                	jne    13ea <phase_4+0x6a>
    13e5:	48 83 c4 18          	add    $0x18,%rsp
    13e9:	c3                   	retq   
    13ea:	e8 61 fa ff ff       	callq  e50 <__stack_chk_fail@plt>

00000000000013ef <phase_5>:
    13ef:	48 83 ec 18          	sub    $0x18,%rsp
    13f3:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    13fa:	00 00 
    13fc:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
    1401:	31 c0                	xor    %eax,%eax
    1403:	48 8d 4c 24 04       	lea    0x4(%rsp),%rcx
    1408:	48 89 e2             	mov    %rsp,%rdx
    140b:	48 8d 35 bd 15 00 00 	lea    0x15bd(%rip),%rsi        # 29cf <array.3413+0x14f>
    1412:	e8 d9 fa ff ff       	callq  ef0 <__isoc99_sscanf@plt>
    1417:	83 f8 01             	cmp    $0x1,%eax
    141a:	7e 5a                	jle    1476 <phase_5+0x87>
    141c:	8b 04 24             	mov    (%rsp),%eax
    141f:	83 e0 0f             	and    $0xf,%eax
    1422:	89 04 24             	mov    %eax,(%rsp)
    1425:	83 f8 0f             	cmp    $0xf,%eax
    1428:	74 32                	je     145c <phase_5+0x6d>
    142a:	b9 00 00 00 00       	mov    $0x0,%ecx
    142f:	ba 00 00 00 00       	mov    $0x0,%edx
    1434:	48 8d 35 45 14 00 00 	lea    0x1445(%rip),%rsi        # 2880 <array.3413>
    143b:	83 c2 01             	add    $0x1,%edx
    143e:	48 98                	cltq   
    1440:	8b 04 86             	mov    (%rsi,%rax,4),%eax
    1443:	01 c1                	add    %eax,%ecx
    1445:	83 f8 0f             	cmp    $0xf,%eax
    1448:	75 f1                	jne    143b <phase_5+0x4c>
    144a:	c7 04 24 0f 00 00 00 	movl   $0xf,(%rsp)
    1451:	83 fa 0f             	cmp    $0xf,%edx
    1454:	75 06                	jne    145c <phase_5+0x6d>
    1456:	39 4c 24 04          	cmp    %ecx,0x4(%rsp)
    145a:	74 05                	je     1461 <phase_5+0x72>
    145c:	e8 86 03 00 00       	callq  17e7 <explode_bomb>
    1461:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
    1466:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    146d:	00 00 
    146f:	75 0c                	jne    147d <phase_5+0x8e>
    1471:	48 83 c4 18          	add    $0x18,%rsp
    1475:	c3                   	retq   
    1476:	e8 6c 03 00 00       	callq  17e7 <explode_bomb>
    147b:	eb 9f                	jmp    141c <phase_5+0x2d>
    147d:	e8 ce f9 ff ff       	callq  e50 <__stack_chk_fail@plt>

0000000000001482 <phase_6>:
    1482:	41 55                	push   %r13
    1484:	41 54                	push   %r12
    1486:	55                   	push   %rbp
    1487:	53                   	push   %rbx
    1488:	48 83 ec 68          	sub    $0x68,%rsp
    148c:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1493:	00 00 
    1495:	48 89 44 24 58       	mov    %rax,0x58(%rsp)
    149a:	31 c0                	xor    %eax,%eax
    149c:	49 89 e4             	mov    %rsp,%r12
    149f:	4c 89 e6             	mov    %r12,%rsi
    14a2:	e8 66 03 00 00       	callq  180d <read_six_numbers>
    14a7:	41 bd 00 00 00 00    	mov    $0x0,%r13d
    14ad:	eb 25                	jmp    14d4 <phase_6+0x52>
    14af:	e8 33 03 00 00       	callq  17e7 <explode_bomb>
    14b4:	eb 2d                	jmp    14e3 <phase_6+0x61>
    14b6:	83 c3 01             	add    $0x1,%ebx
    14b9:	83 fb 05             	cmp    $0x5,%ebx
    14bc:	7f 12                	jg     14d0 <phase_6+0x4e>
    14be:	48 63 c3             	movslq %ebx,%rax
    14c1:	8b 04 84             	mov    (%rsp,%rax,4),%eax
    14c4:	39 45 00             	cmp    %eax,0x0(%rbp)
    14c7:	75 ed                	jne    14b6 <phase_6+0x34>
    14c9:	e8 19 03 00 00       	callq  17e7 <explode_bomb>
    14ce:	eb e6                	jmp    14b6 <phase_6+0x34>
    14d0:	49 83 c4 04          	add    $0x4,%r12
    14d4:	4c 89 e5             	mov    %r12,%rbp
    14d7:	41 8b 04 24          	mov    (%r12),%eax
    14db:	83 e8 01             	sub    $0x1,%eax
    14de:	83 f8 05             	cmp    $0x5,%eax
    14e1:	77 cc                	ja     14af <phase_6+0x2d>
    14e3:	41 83 c5 01          	add    $0x1,%r13d
    14e7:	41 83 fd 06          	cmp    $0x6,%r13d
    14eb:	74 35                	je     1522 <phase_6+0xa0>
    14ed:	44 89 eb             	mov    %r13d,%ebx
    14f0:	eb cc                	jmp    14be <phase_6+0x3c>
    14f2:	48 8b 52 08          	mov    0x8(%rdx),%rdx
    14f6:	83 c0 01             	add    $0x1,%eax
    14f9:	39 c8                	cmp    %ecx,%eax
    14fb:	75 f5                	jne    14f2 <phase_6+0x70>
    14fd:	48 89 54 f4 20       	mov    %rdx,0x20(%rsp,%rsi,8)
    1502:	48 83 c6 01          	add    $0x1,%rsi
    1506:	48 83 fe 06          	cmp    $0x6,%rsi
    150a:	74 1d                	je     1529 <phase_6+0xa7>
    150c:	8b 0c b4             	mov    (%rsp,%rsi,4),%ecx
    150f:	b8 01 00 00 00       	mov    $0x1,%eax
    1514:	48 8d 15 e5 2c 20 00 	lea    0x202ce5(%rip),%rdx        # 204200 <node1>
    151b:	83 f9 01             	cmp    $0x1,%ecx
    151e:	7f d2                	jg     14f2 <phase_6+0x70>
    1520:	eb db                	jmp    14fd <phase_6+0x7b>
    1522:	be 00 00 00 00       	mov    $0x0,%esi
    1527:	eb e3                	jmp    150c <phase_6+0x8a>
    1529:	48 8b 5c 24 20       	mov    0x20(%rsp),%rbx
    152e:	48 8b 44 24 28       	mov    0x28(%rsp),%rax
    1533:	48 89 43 08          	mov    %rax,0x8(%rbx)
    1537:	48 8b 54 24 30       	mov    0x30(%rsp),%rdx
    153c:	48 89 50 08          	mov    %rdx,0x8(%rax)
    1540:	48 8b 44 24 38       	mov    0x38(%rsp),%rax
    1545:	48 89 42 08          	mov    %rax,0x8(%rdx)
    1549:	48 8b 54 24 40       	mov    0x40(%rsp),%rdx
    154e:	48 89 50 08          	mov    %rdx,0x8(%rax)
    1552:	48 8b 44 24 48       	mov    0x48(%rsp),%rax
    1557:	48 89 42 08          	mov    %rax,0x8(%rdx)
    155b:	48 c7 40 08 00 00 00 	movq   $0x0,0x8(%rax)
    1562:	00 
    1563:	bd 05 00 00 00       	mov    $0x5,%ebp
    1568:	eb 09                	jmp    1573 <phase_6+0xf1>
    156a:	48 8b 5b 08          	mov    0x8(%rbx),%rbx
    156e:	83 ed 01             	sub    $0x1,%ebp
    1571:	74 11                	je     1584 <phase_6+0x102>
    1573:	48 8b 43 08          	mov    0x8(%rbx),%rax
    1577:	8b 00                	mov    (%rax),%eax
    1579:	39 03                	cmp    %eax,(%rbx)
    157b:	7e ed                	jle    156a <phase_6+0xe8>
    157d:	e8 65 02 00 00       	callq  17e7 <explode_bomb>
    1582:	eb e6                	jmp    156a <phase_6+0xe8>
    1584:	48 8b 44 24 58       	mov    0x58(%rsp),%rax
    1589:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    1590:	00 00 
    1592:	75 0b                	jne    159f <phase_6+0x11d>
    1594:	48 83 c4 68          	add    $0x68,%rsp
    1598:	5b                   	pop    %rbx
    1599:	5d                   	pop    %rbp
    159a:	41 5c                	pop    %r12
    159c:	41 5d                	pop    %r13
    159e:	c3                   	retq   
    159f:	e8 ac f8 ff ff       	callq  e50 <__stack_chk_fail@plt>

00000000000015a4 <fun7>:
    15a4:	48 85 ff             	test   %rdi,%rdi
    15a7:	74 34                	je     15dd <fun7+0x39>
    15a9:	48 83 ec 08          	sub    $0x8,%rsp
    15ad:	8b 17                	mov    (%rdi),%edx
    15af:	39 f2                	cmp    %esi,%edx
    15b1:	7f 0e                	jg     15c1 <fun7+0x1d>
    15b3:	b8 00 00 00 00       	mov    $0x0,%eax
    15b8:	39 f2                	cmp    %esi,%edx
    15ba:	75 12                	jne    15ce <fun7+0x2a>
    15bc:	48 83 c4 08          	add    $0x8,%rsp
    15c0:	c3                   	retq   
    15c1:	48 8b 7f 08          	mov    0x8(%rdi),%rdi
    15c5:	e8 da ff ff ff       	callq  15a4 <fun7>
    15ca:	01 c0                	add    %eax,%eax
    15cc:	eb ee                	jmp    15bc <fun7+0x18>
    15ce:	48 8b 7f 10          	mov    0x10(%rdi),%rdi
    15d2:	e8 cd ff ff ff       	callq  15a4 <fun7>
    15d7:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax
    15db:	eb df                	jmp    15bc <fun7+0x18>
    15dd:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    15e2:	c3                   	retq   

00000000000015e3 <secret_phase>:
    15e3:	53                   	push   %rbx
    15e4:	e8 65 02 00 00       	callq  184e <read_line>
    15e9:	ba 0a 00 00 00       	mov    $0xa,%edx
    15ee:	be 00 00 00 00       	mov    $0x0,%esi
    15f3:	48 89 c7             	mov    %rax,%rdi
    15f6:	e8 d5 f8 ff ff       	callq  ed0 <strtol@plt>
    15fb:	48 89 c3             	mov    %rax,%rbx
    15fe:	8d 40 ff             	lea    -0x1(%rax),%eax
    1601:	3d e8 03 00 00       	cmp    $0x3e8,%eax
    1606:	77 25                	ja     162d <secret_phase+0x4a>
    1608:	89 de                	mov    %ebx,%esi
    160a:	48 8d 3d 0f 2b 20 00 	lea    0x202b0f(%rip),%rdi        # 204120 <n1>
    1611:	e8 8e ff ff ff       	callq  15a4 <fun7>
    1616:	85 c0                	test   %eax,%eax
    1618:	75 1a                	jne    1634 <secret_phase+0x51>
    161a:	48 8d 3d 07 12 00 00 	lea    0x1207(%rip),%rdi        # 2828 <_IO_stdin_used+0x188>
    1621:	e8 0a f8 ff ff       	callq  e30 <puts@plt>
    1626:	e8 67 03 00 00       	callq  1992 <phase_defused>
    162b:	5b                   	pop    %rbx
    162c:	c3                   	retq   
    162d:	e8 b5 01 00 00       	callq  17e7 <explode_bomb>
    1632:	eb d4                	jmp    1608 <secret_phase+0x25>
    1634:	e8 ae 01 00 00       	callq  17e7 <explode_bomb>
    1639:	eb df                	jmp    161a <secret_phase+0x37>

000000000000163b <sig_handler>:
    163b:	48 83 ec 08          	sub    $0x8,%rsp
    163f:	48 8d 3d 7a 12 00 00 	lea    0x127a(%rip),%rdi        # 28c0 <array.3413+0x40>
    1646:	e8 e5 f7 ff ff       	callq  e30 <puts@plt>
    164b:	bf 03 00 00 00       	mov    $0x3,%edi
    1650:	e8 fb f8 ff ff       	callq  f50 <sleep@plt>
    1655:	48 8d 35 26 13 00 00 	lea    0x1326(%rip),%rsi        # 2982 <array.3413+0x102>
    165c:	bf 01 00 00 00       	mov    $0x1,%edi
    1661:	b8 00 00 00 00       	mov    $0x0,%eax
    1666:	e8 95 f8 ff ff       	callq  f00 <__printf_chk@plt>
    166b:	48 8b 3d ee 2f 20 00 	mov    0x202fee(%rip),%rdi        # 204660 <stdout@@GLIBC_2.2.5>
    1672:	e8 69 f8 ff ff       	callq  ee0 <fflush@plt>
    1677:	bf 01 00 00 00       	mov    $0x1,%edi
    167c:	e8 cf f8 ff ff       	callq  f50 <sleep@plt>
    1681:	48 8d 3d 02 13 00 00 	lea    0x1302(%rip),%rdi        # 298a <array.3413+0x10a>
    1688:	e8 a3 f7 ff ff       	callq  e30 <puts@plt>
    168d:	bf 10 00 00 00       	mov    $0x10,%edi
    1692:	e8 89 f8 ff ff       	callq  f20 <exit@plt>

0000000000001697 <invalid_phase>:
    1697:	48 83 ec 08          	sub    $0x8,%rsp
    169b:	48 89 fa             	mov    %rdi,%rdx
    169e:	48 8d 35 ed 12 00 00 	lea    0x12ed(%rip),%rsi        # 2992 <array.3413+0x112>
    16a5:	bf 01 00 00 00       	mov    $0x1,%edi
    16aa:	b8 00 00 00 00       	mov    $0x0,%eax
    16af:	e8 4c f8 ff ff       	callq  f00 <__printf_chk@plt>
    16b4:	bf 08 00 00 00       	mov    $0x8,%edi
    16b9:	e8 62 f8 ff ff       	callq  f20 <exit@plt>

00000000000016be <string_length>:
    16be:	80 3f 00             	cmpb   $0x0,(%rdi)
    16c1:	74 12                	je     16d5 <string_length+0x17>
    16c3:	48 89 fa             	mov    %rdi,%rdx
    16c6:	48 83 c2 01          	add    $0x1,%rdx
    16ca:	89 d0                	mov    %edx,%eax
    16cc:	29 f8                	sub    %edi,%eax
    16ce:	80 3a 00             	cmpb   $0x0,(%rdx)
    16d1:	75 f3                	jne    16c6 <string_length+0x8>
    16d3:	f3 c3                	repz retq 
    16d5:	b8 00 00 00 00       	mov    $0x0,%eax
    16da:	c3                   	retq   

00000000000016db <strings_not_equal>:
    16db:	41 54                	push   %r12
    16dd:	55                   	push   %rbp
    16de:	53                   	push   %rbx
    16df:	48 89 fb             	mov    %rdi,%rbx
    16e2:	48 89 f5             	mov    %rsi,%rbp
    16e5:	e8 d4 ff ff ff       	callq  16be <string_length>
    16ea:	41 89 c4             	mov    %eax,%r12d
    16ed:	48 89 ef             	mov    %rbp,%rdi
    16f0:	e8 c9 ff ff ff       	callq  16be <string_length>
    16f5:	ba 01 00 00 00       	mov    $0x1,%edx
    16fa:	41 39 c4             	cmp    %eax,%r12d
    16fd:	74 07                	je     1706 <strings_not_equal+0x2b>
    16ff:	89 d0                	mov    %edx,%eax
    1701:	5b                   	pop    %rbx
    1702:	5d                   	pop    %rbp
    1703:	41 5c                	pop    %r12
    1705:	c3                   	retq   
    1706:	0f b6 03             	movzbl (%rbx),%eax
    1709:	84 c0                	test   %al,%al
    170b:	74 27                	je     1734 <strings_not_equal+0x59>
    170d:	3a 45 00             	cmp    0x0(%rbp),%al
    1710:	75 29                	jne    173b <strings_not_equal+0x60>
    1712:	48 83 c3 01          	add    $0x1,%rbx
    1716:	48 83 c5 01          	add    $0x1,%rbp
    171a:	0f b6 03             	movzbl (%rbx),%eax
    171d:	84 c0                	test   %al,%al
    171f:	74 0c                	je     172d <strings_not_equal+0x52>
    1721:	38 45 00             	cmp    %al,0x0(%rbp)
    1724:	74 ec                	je     1712 <strings_not_equal+0x37>
    1726:	ba 01 00 00 00       	mov    $0x1,%edx
    172b:	eb d2                	jmp    16ff <strings_not_equal+0x24>
    172d:	ba 00 00 00 00       	mov    $0x0,%edx
    1732:	eb cb                	jmp    16ff <strings_not_equal+0x24>
    1734:	ba 00 00 00 00       	mov    $0x0,%edx
    1739:	eb c4                	jmp    16ff <strings_not_equal+0x24>
    173b:	ba 01 00 00 00       	mov    $0x1,%edx
    1740:	eb bd                	jmp    16ff <strings_not_equal+0x24>

0000000000001742 <initialize_bomb>:
    1742:	48 83 ec 08          	sub    $0x8,%rsp
    1746:	48 8d 35 ee fe ff ff 	lea    -0x112(%rip),%rsi        # 163b <sig_handler>
    174d:	bf 02 00 00 00       	mov    $0x2,%edi
    1752:	e8 49 f7 ff ff       	callq  ea0 <signal@plt>
    1757:	48 83 c4 08          	add    $0x8,%rsp
    175b:	c3                   	retq   

000000000000175c <initialize_bomb_solve>:
    175c:	f3 c3                	repz retq 

000000000000175e <blank_line>:
    175e:	55                   	push   %rbp
    175f:	53                   	push   %rbx
    1760:	48 83 ec 08          	sub    $0x8,%rsp
    1764:	48 89 fd             	mov    %rdi,%rbp
    1767:	0f b6 5d 00          	movzbl 0x0(%rbp),%ebx
    176b:	84 db                	test   %bl,%bl
    176d:	74 1e                	je     178d <blank_line+0x2f>
    176f:	e8 ec f7 ff ff       	callq  f60 <__ctype_b_loc@plt>
    1774:	48 83 c5 01          	add    $0x1,%rbp
    1778:	48 0f be db          	movsbq %bl,%rbx
    177c:	48 8b 00             	mov    (%rax),%rax
    177f:	f6 44 58 01 20       	testb  $0x20,0x1(%rax,%rbx,2)
    1784:	75 e1                	jne    1767 <blank_line+0x9>
    1786:	b8 00 00 00 00       	mov    $0x0,%eax
    178b:	eb 05                	jmp    1792 <blank_line+0x34>
    178d:	b8 01 00 00 00       	mov    $0x1,%eax
    1792:	48 83 c4 08          	add    $0x8,%rsp
    1796:	5b                   	pop    %rbx
    1797:	5d                   	pop    %rbp
    1798:	c3                   	retq   

0000000000001799 <skip>:
    1799:	55                   	push   %rbp
    179a:	53                   	push   %rbx
    179b:	48 83 ec 08          	sub    $0x8,%rsp
    179f:	48 8d 2d fa 2e 20 00 	lea    0x202efa(%rip),%rbp        # 2046a0 <input_strings>
    17a6:	48 63 05 e3 2e 20 00 	movslq 0x202ee3(%rip),%rax        # 204690 <num_input_strings>
    17ad:	48 8d 3c 80          	lea    (%rax,%rax,4),%rdi
    17b1:	48 c1 e7 04          	shl    $0x4,%rdi
    17b5:	48 01 ef             	add    %rbp,%rdi
    17b8:	48 8b 15 d9 2e 20 00 	mov    0x202ed9(%rip),%rdx        # 204698 <infile>
    17bf:	be 50 00 00 00       	mov    $0x50,%esi
    17c4:	e8 c7 f6 ff ff       	callq  e90 <fgets@plt>
    17c9:	48 89 c3             	mov    %rax,%rbx
    17cc:	48 85 c0             	test   %rax,%rax
    17cf:	74 0c                	je     17dd <skip+0x44>
    17d1:	48 89 c7             	mov    %rax,%rdi
    17d4:	e8 85 ff ff ff       	callq  175e <blank_line>
    17d9:	85 c0                	test   %eax,%eax
    17db:	75 c9                	jne    17a6 <skip+0xd>
    17dd:	48 89 d8             	mov    %rbx,%rax
    17e0:	48 83 c4 08          	add    $0x8,%rsp
    17e4:	5b                   	pop    %rbx
    17e5:	5d                   	pop    %rbp
    17e6:	c3                   	retq   

00000000000017e7 <explode_bomb>:
    17e7:	48 83 ec 08          	sub    $0x8,%rsp
    17eb:	48 8d 3d b1 11 00 00 	lea    0x11b1(%rip),%rdi        # 29a3 <array.3413+0x123>
    17f2:	e8 39 f6 ff ff       	callq  e30 <puts@plt>
    17f7:	48 8d 3d ae 11 00 00 	lea    0x11ae(%rip),%rdi        # 29ac <array.3413+0x12c>
    17fe:	e8 2d f6 ff ff       	callq  e30 <puts@plt>
    1803:	bf 08 00 00 00       	mov    $0x8,%edi
    1808:	e8 13 f7 ff ff       	callq  f20 <exit@plt>

000000000000180d <read_six_numbers>:
    180d:	48 83 ec 08          	sub    $0x8,%rsp
    1811:	48 89 f2             	mov    %rsi,%rdx
    1814:	48 8d 4e 04          	lea    0x4(%rsi),%rcx
    1818:	48 8d 46 14          	lea    0x14(%rsi),%rax
    181c:	50                   	push   %rax
    181d:	48 8d 46 10          	lea    0x10(%rsi),%rax
    1821:	50                   	push   %rax
    1822:	4c 8d 4e 0c          	lea    0xc(%rsi),%r9
    1826:	4c 8d 46 08          	lea    0x8(%rsi),%r8
    182a:	48 8d 35 92 11 00 00 	lea    0x1192(%rip),%rsi        # 29c3 <array.3413+0x143>
    1831:	b8 00 00 00 00       	mov    $0x0,%eax
    1836:	e8 b5 f6 ff ff       	callq  ef0 <__isoc99_sscanf@plt>
    183b:	48 83 c4 10          	add    $0x10,%rsp
    183f:	83 f8 05             	cmp    $0x5,%eax
    1842:	7e 05                	jle    1849 <read_six_numbers+0x3c>
    1844:	48 83 c4 08          	add    $0x8,%rsp
    1848:	c3                   	retq   
    1849:	e8 99 ff ff ff       	callq  17e7 <explode_bomb>

000000000000184e <read_line>:
    184e:	48 83 ec 08          	sub    $0x8,%rsp
    1852:	b8 00 00 00 00       	mov    $0x0,%eax
    1857:	e8 3d ff ff ff       	callq  1799 <skip>
    185c:	48 85 c0             	test   %rax,%rax
    185f:	74 6f                	je     18d0 <read_line+0x82>
    1861:	8b 35 29 2e 20 00    	mov    0x202e29(%rip),%esi        # 204690 <num_input_strings>
    1867:	48 63 c6             	movslq %esi,%rax
    186a:	48 8d 14 80          	lea    (%rax,%rax,4),%rdx
    186e:	48 c1 e2 04          	shl    $0x4,%rdx
    1872:	48 8d 05 27 2e 20 00 	lea    0x202e27(%rip),%rax        # 2046a0 <input_strings>
    1879:	48 01 c2             	add    %rax,%rdx
    187c:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
    1883:	b8 00 00 00 00       	mov    $0x0,%eax
    1888:	48 89 d7             	mov    %rdx,%rdi
    188b:	f2 ae                	repnz scas %es:(%rdi),%al
    188d:	48 f7 d1             	not    %rcx
    1890:	48 83 e9 01          	sub    $0x1,%rcx
    1894:	83 f9 4e             	cmp    $0x4e,%ecx
    1897:	0f 8f ab 00 00 00    	jg     1948 <read_line+0xfa>
    189d:	83 e9 01             	sub    $0x1,%ecx
    18a0:	48 63 c9             	movslq %ecx,%rcx
    18a3:	48 63 c6             	movslq %esi,%rax
    18a6:	48 8d 04 80          	lea    (%rax,%rax,4),%rax
    18aa:	48 c1 e0 04          	shl    $0x4,%rax
    18ae:	48 89 c7             	mov    %rax,%rdi
    18b1:	48 8d 05 e8 2d 20 00 	lea    0x202de8(%rip),%rax        # 2046a0 <input_strings>
    18b8:	48 01 f8             	add    %rdi,%rax
    18bb:	c6 04 08 00          	movb   $0x0,(%rax,%rcx,1)
    18bf:	83 c6 01             	add    $0x1,%esi
    18c2:	89 35 c8 2d 20 00    	mov    %esi,0x202dc8(%rip)        # 204690 <num_input_strings>
    18c8:	48 89 d0             	mov    %rdx,%rax
    18cb:	48 83 c4 08          	add    $0x8,%rsp
    18cf:	c3                   	retq   
    18d0:	48 8b 05 99 2d 20 00 	mov    0x202d99(%rip),%rax        # 204670 <stdin@@GLIBC_2.2.5>
    18d7:	48 39 05 ba 2d 20 00 	cmp    %rax,0x202dba(%rip)        # 204698 <infile>
    18de:	74 1b                	je     18fb <read_line+0xad>
    18e0:	48 8d 3d 0c 11 00 00 	lea    0x110c(%rip),%rdi        # 29f3 <array.3413+0x173>
    18e7:	e8 14 f5 ff ff       	callq  e00 <getenv@plt>
    18ec:	48 85 c0             	test   %rax,%rax
    18ef:	74 20                	je     1911 <read_line+0xc3>
    18f1:	bf 00 00 00 00       	mov    $0x0,%edi
    18f6:	e8 25 f6 ff ff       	callq  f20 <exit@plt>
    18fb:	48 8d 3d d3 10 00 00 	lea    0x10d3(%rip),%rdi        # 29d5 <array.3413+0x155>
    1902:	e8 29 f5 ff ff       	callq  e30 <puts@plt>
    1907:	bf 08 00 00 00       	mov    $0x8,%edi
    190c:	e8 0f f6 ff ff       	callq  f20 <exit@plt>
    1911:	48 8b 05 58 2d 20 00 	mov    0x202d58(%rip),%rax        # 204670 <stdin@@GLIBC_2.2.5>
    1918:	48 89 05 79 2d 20 00 	mov    %rax,0x202d79(%rip)        # 204698 <infile>
    191f:	b8 00 00 00 00       	mov    $0x0,%eax
    1924:	e8 70 fe ff ff       	callq  1799 <skip>
    1929:	48 85 c0             	test   %rax,%rax
    192c:	0f 85 2f ff ff ff    	jne    1861 <read_line+0x13>
    1932:	48 8d 3d 9c 10 00 00 	lea    0x109c(%rip),%rdi        # 29d5 <array.3413+0x155>
    1939:	e8 f2 f4 ff ff       	callq  e30 <puts@plt>
    193e:	bf 00 00 00 00       	mov    $0x0,%edi
    1943:	e8 d8 f5 ff ff       	callq  f20 <exit@plt>
    1948:	48 8d 3d af 10 00 00 	lea    0x10af(%rip),%rdi        # 29fe <array.3413+0x17e>
    194f:	e8 dc f4 ff ff       	callq  e30 <puts@plt>
    1954:	8b 05 36 2d 20 00    	mov    0x202d36(%rip),%eax        # 204690 <num_input_strings>
    195a:	8d 50 01             	lea    0x1(%rax),%edx
    195d:	89 15 2d 2d 20 00    	mov    %edx,0x202d2d(%rip)        # 204690 <num_input_strings>
    1963:	48 98                	cltq   
    1965:	48 6b c0 50          	imul   $0x50,%rax,%rax
    1969:	48 8d 15 30 2d 20 00 	lea    0x202d30(%rip),%rdx        # 2046a0 <input_strings>
    1970:	48 be 2a 2a 2a 74 72 	movabs $0x636e7572742a2a2a,%rsi
    1977:	75 6e 63 
    197a:	48 bf 61 74 65 64 2a 	movabs $0x2a2a2a64657461,%rdi
    1981:	2a 2a 00 
    1984:	48 89 34 02          	mov    %rsi,(%rdx,%rax,1)
    1988:	48 89 7c 02 08       	mov    %rdi,0x8(%rdx,%rax,1)
    198d:	e8 55 fe ff ff       	callq  17e7 <explode_bomb>

0000000000001992 <phase_defused>:
    1992:	48 83 ec 78          	sub    $0x78,%rsp
    1996:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    199d:	00 00 
    199f:	48 89 44 24 68       	mov    %rax,0x68(%rsp)
    19a4:	31 c0                	xor    %eax,%eax
    19a6:	83 3d e3 2c 20 00 06 	cmpl   $0x6,0x202ce3(%rip)        # 204690 <num_input_strings>
    19ad:	74 15                	je     19c4 <phase_defused+0x32>
    19af:	48 8b 44 24 68       	mov    0x68(%rsp),%rax
    19b4:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    19bb:	00 00 
    19bd:	75 73                	jne    1a32 <phase_defused+0xa0>
    19bf:	48 83 c4 78          	add    $0x78,%rsp
    19c3:	c3                   	retq   
    19c4:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
    19c9:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
    19ce:	4c 8d 44 24 10       	lea    0x10(%rsp),%r8
    19d3:	48 8d 35 3f 10 00 00 	lea    0x103f(%rip),%rsi        # 2a19 <array.3413+0x199>
    19da:	48 8d 3d af 2d 20 00 	lea    0x202daf(%rip),%rdi        # 204790 <input_strings+0xf0>
    19e1:	e8 0a f5 ff ff       	callq  ef0 <__isoc99_sscanf@plt>
    19e6:	83 f8 03             	cmp    $0x3,%eax
    19e9:	74 0e                	je     19f9 <phase_defused+0x67>
    19eb:	48 8d 3d 66 0f 00 00 	lea    0xf66(%rip),%rdi        # 2958 <array.3413+0xd8>
    19f2:	e8 39 f4 ff ff       	callq  e30 <puts@plt>
    19f7:	eb b6                	jmp    19af <phase_defused+0x1d>
    19f9:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
    19fe:	48 8d 35 1d 10 00 00 	lea    0x101d(%rip),%rsi        # 2a22 <array.3413+0x1a2>
    1a05:	e8 d1 fc ff ff       	callq  16db <strings_not_equal>
    1a0a:	85 c0                	test   %eax,%eax
    1a0c:	75 dd                	jne    19eb <phase_defused+0x59>
    1a0e:	48 8d 3d e3 0e 00 00 	lea    0xee3(%rip),%rdi        # 28f8 <array.3413+0x78>
    1a15:	e8 16 f4 ff ff       	callq  e30 <puts@plt>
    1a1a:	48 8d 3d ff 0e 00 00 	lea    0xeff(%rip),%rdi        # 2920 <array.3413+0xa0>
    1a21:	e8 0a f4 ff ff       	callq  e30 <puts@plt>
    1a26:	b8 00 00 00 00       	mov    $0x0,%eax
    1a2b:	e8 b3 fb ff ff       	callq  15e3 <secret_phase>
    1a30:	eb b9                	jmp    19eb <phase_defused+0x59>
    1a32:	e8 19 f4 ff ff       	callq  e50 <__stack_chk_fail@plt>

0000000000001a37 <sigalrm_handler>:
    1a37:	48 83 ec 08          	sub    $0x8,%rsp
    1a3b:	b9 00 00 00 00       	mov    $0x0,%ecx
    1a40:	48 8d 15 49 10 00 00 	lea    0x1049(%rip),%rdx        # 2a90 <array.3413+0x210>
    1a47:	be 01 00 00 00       	mov    $0x1,%esi
    1a4c:	48 8b 3d 2d 2c 20 00 	mov    0x202c2d(%rip),%rdi        # 204680 <stderr@@GLIBC_2.2.5>
    1a53:	b8 00 00 00 00       	mov    $0x0,%eax
    1a58:	e8 e3 f4 ff ff       	callq  f40 <__fprintf_chk@plt>
    1a5d:	bf 01 00 00 00       	mov    $0x1,%edi
    1a62:	e8 b9 f4 ff ff       	callq  f20 <exit@plt>

0000000000001a67 <rio_readlineb>:
    1a67:	41 56                	push   %r14
    1a69:	41 55                	push   %r13
    1a6b:	41 54                	push   %r12
    1a6d:	55                   	push   %rbp
    1a6e:	53                   	push   %rbx
    1a6f:	48 89 fb             	mov    %rdi,%rbx
    1a72:	49 89 f4             	mov    %rsi,%r12
    1a75:	49 89 d6             	mov    %rdx,%r14
    1a78:	41 bd 01 00 00 00    	mov    $0x1,%r13d
    1a7e:	48 8d 6f 10          	lea    0x10(%rdi),%rbp
    1a82:	48 83 fa 01          	cmp    $0x1,%rdx
    1a86:	77 0c                	ja     1a94 <rio_readlineb+0x2d>
    1a88:	eb 60                	jmp    1aea <rio_readlineb+0x83>
    1a8a:	e8 81 f3 ff ff       	callq  e10 <__errno_location@plt>
    1a8f:	83 38 04             	cmpl   $0x4,(%rax)
    1a92:	75 67                	jne    1afb <rio_readlineb+0x94>
    1a94:	8b 43 04             	mov    0x4(%rbx),%eax
    1a97:	85 c0                	test   %eax,%eax
    1a99:	7f 20                	jg     1abb <rio_readlineb+0x54>
    1a9b:	ba 00 20 00 00       	mov    $0x2000,%edx
    1aa0:	48 89 ee             	mov    %rbp,%rsi
    1aa3:	8b 3b                	mov    (%rbx),%edi
    1aa5:	e8 d6 f3 ff ff       	callq  e80 <read@plt>
    1aaa:	89 43 04             	mov    %eax,0x4(%rbx)
    1aad:	85 c0                	test   %eax,%eax
    1aaf:	78 d9                	js     1a8a <rio_readlineb+0x23>
    1ab1:	85 c0                	test   %eax,%eax
    1ab3:	74 4f                	je     1b04 <rio_readlineb+0x9d>
    1ab5:	48 89 6b 08          	mov    %rbp,0x8(%rbx)
    1ab9:	eb d9                	jmp    1a94 <rio_readlineb+0x2d>
    1abb:	48 8b 53 08          	mov    0x8(%rbx),%rdx
    1abf:	0f b6 0a             	movzbl (%rdx),%ecx
    1ac2:	48 83 c2 01          	add    $0x1,%rdx
    1ac6:	48 89 53 08          	mov    %rdx,0x8(%rbx)
    1aca:	83 e8 01             	sub    $0x1,%eax
    1acd:	89 43 04             	mov    %eax,0x4(%rbx)
    1ad0:	49 83 c4 01          	add    $0x1,%r12
    1ad4:	41 88 4c 24 ff       	mov    %cl,-0x1(%r12)
    1ad9:	80 f9 0a             	cmp    $0xa,%cl
    1adc:	74 0c                	je     1aea <rio_readlineb+0x83>
    1ade:	41 83 c5 01          	add    $0x1,%r13d
    1ae2:	49 63 c5             	movslq %r13d,%rax
    1ae5:	4c 39 f0             	cmp    %r14,%rax
    1ae8:	72 aa                	jb     1a94 <rio_readlineb+0x2d>
    1aea:	41 c6 04 24 00       	movb   $0x0,(%r12)
    1aef:	49 63 c5             	movslq %r13d,%rax
    1af2:	5b                   	pop    %rbx
    1af3:	5d                   	pop    %rbp
    1af4:	41 5c                	pop    %r12
    1af6:	41 5d                	pop    %r13
    1af8:	41 5e                	pop    %r14
    1afa:	c3                   	retq   
    1afb:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
    1b02:	eb 05                	jmp    1b09 <rio_readlineb+0xa2>
    1b04:	b8 00 00 00 00       	mov    $0x0,%eax
    1b09:	85 c0                	test   %eax,%eax
    1b0b:	75 0d                	jne    1b1a <rio_readlineb+0xb3>
    1b0d:	b8 00 00 00 00       	mov    $0x0,%eax
    1b12:	41 83 fd 01          	cmp    $0x1,%r13d
    1b16:	75 d2                	jne    1aea <rio_readlineb+0x83>
    1b18:	eb d8                	jmp    1af2 <rio_readlineb+0x8b>
    1b1a:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
    1b21:	eb cf                	jmp    1af2 <rio_readlineb+0x8b>

0000000000001b23 <submitr>:
    1b23:	41 57                	push   %r15
    1b25:	41 56                	push   %r14
    1b27:	41 55                	push   %r13
    1b29:	41 54                	push   %r12
    1b2b:	55                   	push   %rbp
    1b2c:	53                   	push   %rbx
    1b2d:	48 81 ec 78 a0 00 00 	sub    $0xa078,%rsp
    1b34:	49 89 fd             	mov    %rdi,%r13
    1b37:	89 f5                	mov    %esi,%ebp
    1b39:	48 89 54 24 08       	mov    %rdx,0x8(%rsp)
    1b3e:	48 89 4c 24 10       	mov    %rcx,0x10(%rsp)
    1b43:	4c 89 44 24 20       	mov    %r8,0x20(%rsp)
    1b48:	4c 89 4c 24 18       	mov    %r9,0x18(%rsp)
    1b4d:	48 8b 9c 24 b0 a0 00 	mov    0xa0b0(%rsp),%rbx
    1b54:	00 
    1b55:	4c 8b bc 24 b8 a0 00 	mov    0xa0b8(%rsp),%r15
    1b5c:	00 
    1b5d:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1b64:	00 00 
    1b66:	48 89 84 24 68 a0 00 	mov    %rax,0xa068(%rsp)
    1b6d:	00 
    1b6e:	31 c0                	xor    %eax,%eax
    1b70:	c7 44 24 3c 00 00 00 	movl   $0x0,0x3c(%rsp)
    1b77:	00 
    1b78:	ba 00 00 00 00       	mov    $0x0,%edx
    1b7d:	be 01 00 00 00       	mov    $0x1,%esi
    1b82:	bf 02 00 00 00       	mov    $0x2,%edi
    1b87:	e8 f4 f3 ff ff       	callq  f80 <socket@plt>
    1b8c:	85 c0                	test   %eax,%eax
    1b8e:	0f 88 35 01 00 00    	js     1cc9 <submitr+0x1a6>
    1b94:	41 89 c4             	mov    %eax,%r12d
    1b97:	4c 89 ef             	mov    %r13,%rdi
    1b9a:	e8 11 f3 ff ff       	callq  eb0 <gethostbyname@plt>
    1b9f:	48 85 c0             	test   %rax,%rax
    1ba2:	0f 84 71 01 00 00    	je     1d19 <submitr+0x1f6>
    1ba8:	4c 8d 6c 24 40       	lea    0x40(%rsp),%r13
    1bad:	48 c7 44 24 42 00 00 	movq   $0x0,0x42(%rsp)
    1bb4:	00 00 
    1bb6:	c7 44 24 4a 00 00 00 	movl   $0x0,0x4a(%rsp)
    1bbd:	00 
    1bbe:	66 c7 44 24 4e 00 00 	movw   $0x0,0x4e(%rsp)
    1bc5:	66 c7 44 24 40 02 00 	movw   $0x2,0x40(%rsp)
    1bcc:	48 63 50 14          	movslq 0x14(%rax),%rdx
    1bd0:	48 8b 40 18          	mov    0x18(%rax),%rax
    1bd4:	48 8d 7c 24 44       	lea    0x44(%rsp),%rdi
    1bd9:	b9 0c 00 00 00       	mov    $0xc,%ecx
    1bde:	48 8b 30             	mov    (%rax),%rsi
    1be1:	e8 da f2 ff ff       	callq  ec0 <__memmove_chk@plt>
    1be6:	66 c1 cd 08          	ror    $0x8,%bp
    1bea:	66 89 6c 24 42       	mov    %bp,0x42(%rsp)
    1bef:	ba 10 00 00 00       	mov    $0x10,%edx
    1bf4:	4c 89 ee             	mov    %r13,%rsi
    1bf7:	44 89 e7             	mov    %r12d,%edi
    1bfa:	e8 31 f3 ff ff       	callq  f30 <connect@plt>
    1bff:	85 c0                	test   %eax,%eax
    1c01:	0f 88 7d 01 00 00    	js     1d84 <submitr+0x261>
    1c07:	49 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%r9
    1c0e:	b8 00 00 00 00       	mov    $0x0,%eax
    1c13:	4c 89 c9             	mov    %r9,%rcx
    1c16:	48 89 df             	mov    %rbx,%rdi
    1c19:	f2 ae                	repnz scas %es:(%rdi),%al
    1c1b:	48 89 ce             	mov    %rcx,%rsi
    1c1e:	48 f7 d6             	not    %rsi
    1c21:	4c 89 c9             	mov    %r9,%rcx
    1c24:	48 8b 7c 24 08       	mov    0x8(%rsp),%rdi
    1c29:	f2 ae                	repnz scas %es:(%rdi),%al
    1c2b:	49 89 c8             	mov    %rcx,%r8
    1c2e:	4c 89 c9             	mov    %r9,%rcx
    1c31:	48 8b 7c 24 10       	mov    0x10(%rsp),%rdi
    1c36:	f2 ae                	repnz scas %es:(%rdi),%al
    1c38:	48 89 ca             	mov    %rcx,%rdx
    1c3b:	48 f7 d2             	not    %rdx
    1c3e:	4c 89 c9             	mov    %r9,%rcx
    1c41:	48 8b 7c 24 18       	mov    0x18(%rsp),%rdi
    1c46:	f2 ae                	repnz scas %es:(%rdi),%al
    1c48:	4c 29 c2             	sub    %r8,%rdx
    1c4b:	48 29 ca             	sub    %rcx,%rdx
    1c4e:	48 8d 44 76 fd       	lea    -0x3(%rsi,%rsi,2),%rax
    1c53:	48 8d 44 02 7b       	lea    0x7b(%rdx,%rax,1),%rax
    1c58:	48 3d 00 20 00 00    	cmp    $0x2000,%rax
    1c5e:	0f 87 7d 01 00 00    	ja     1de1 <submitr+0x2be>
    1c64:	48 8d 94 24 60 40 00 	lea    0x4060(%rsp),%rdx
    1c6b:	00 
    1c6c:	b9 00 04 00 00       	mov    $0x400,%ecx
    1c71:	b8 00 00 00 00       	mov    $0x0,%eax
    1c76:	48 89 d7             	mov    %rdx,%rdi
    1c79:	f3 48 ab             	rep stos %rax,%es:(%rdi)
    1c7c:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
    1c83:	48 89 df             	mov    %rbx,%rdi
    1c86:	f2 ae                	repnz scas %es:(%rdi),%al
    1c88:	48 89 ca             	mov    %rcx,%rdx
    1c8b:	48 f7 d2             	not    %rdx
    1c8e:	48 89 d1             	mov    %rdx,%rcx
    1c91:	48 83 e9 01          	sub    $0x1,%rcx
    1c95:	85 c9                	test   %ecx,%ecx
    1c97:	0f 84 3f 06 00 00    	je     22dc <submitr+0x7b9>
    1c9d:	8d 41 ff             	lea    -0x1(%rcx),%eax
    1ca0:	4c 8d 74 03 01       	lea    0x1(%rbx,%rax,1),%r14
    1ca5:	48 8d ac 24 60 40 00 	lea    0x4060(%rsp),%rbp
    1cac:	00 
    1cad:	48 8d 84 24 60 80 00 	lea    0x8060(%rsp),%rax
    1cb4:	00 
    1cb5:	48 89 44 24 28       	mov    %rax,0x28(%rsp)
    1cba:	49 bd d9 ff 00 00 00 	movabs $0x2000000000ffd9,%r13
    1cc1:	00 20 00 
    1cc4:	e9 a6 01 00 00       	jmpq   1e6f <submitr+0x34c>
    1cc9:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    1cd0:	3a 20 43 
    1cd3:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    1cda:	20 75 6e 
    1cdd:	49 89 07             	mov    %rax,(%r15)
    1ce0:	49 89 57 08          	mov    %rdx,0x8(%r15)
    1ce4:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    1ceb:	74 6f 20 
    1cee:	48 ba 63 72 65 61 74 	movabs $0x7320657461657263,%rdx
    1cf5:	65 20 73 
    1cf8:	49 89 47 10          	mov    %rax,0x10(%r15)
    1cfc:	49 89 57 18          	mov    %rdx,0x18(%r15)
    1d00:	41 c7 47 20 6f 63 6b 	movl   $0x656b636f,0x20(%r15)
    1d07:	65 
    1d08:	66 41 c7 47 24 74 00 	movw   $0x74,0x24(%r15)
    1d0f:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    1d14:	e9 9a 04 00 00       	jmpq   21b3 <submitr+0x690>
    1d19:	48 b8 45 72 72 6f 72 	movabs $0x44203a726f727245,%rax
    1d20:	3a 20 44 
    1d23:	48 ba 4e 53 20 69 73 	movabs $0x6e7520736920534e,%rdx
    1d2a:	20 75 6e 
    1d2d:	49 89 07             	mov    %rax,(%r15)
    1d30:	49 89 57 08          	mov    %rdx,0x8(%r15)
    1d34:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    1d3b:	74 6f 20 
    1d3e:	48 ba 72 65 73 6f 6c 	movabs $0x2065766c6f736572,%rdx
    1d45:	76 65 20 
    1d48:	49 89 47 10          	mov    %rax,0x10(%r15)
    1d4c:	49 89 57 18          	mov    %rdx,0x18(%r15)
    1d50:	48 b8 73 65 72 76 65 	movabs $0x6120726576726573,%rax
    1d57:	72 20 61 
    1d5a:	49 89 47 20          	mov    %rax,0x20(%r15)
    1d5e:	41 c7 47 28 64 64 72 	movl   $0x65726464,0x28(%r15)
    1d65:	65 
    1d66:	66 41 c7 47 2c 73 73 	movw   $0x7373,0x2c(%r15)
    1d6d:	41 c6 47 2e 00       	movb   $0x0,0x2e(%r15)
    1d72:	44 89 e7             	mov    %r12d,%edi
    1d75:	e8 f6 f0 ff ff       	callq  e70 <close@plt>
    1d7a:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    1d7f:	e9 2f 04 00 00       	jmpq   21b3 <submitr+0x690>
    1d84:	48 b8 45 72 72 6f 72 	movabs $0x55203a726f727245,%rax
    1d8b:	3a 20 55 
    1d8e:	48 ba 6e 61 62 6c 65 	movabs $0x6f7420656c62616e,%rdx
    1d95:	20 74 6f 
    1d98:	49 89 07             	mov    %rax,(%r15)
    1d9b:	49 89 57 08          	mov    %rdx,0x8(%r15)
    1d9f:	48 b8 20 63 6f 6e 6e 	movabs $0x7463656e6e6f6320,%rax
    1da6:	65 63 74 
    1da9:	48 ba 20 74 6f 20 74 	movabs $0x20656874206f7420,%rdx
    1db0:	68 65 20 
    1db3:	49 89 47 10          	mov    %rax,0x10(%r15)
    1db7:	49 89 57 18          	mov    %rdx,0x18(%r15)
    1dbb:	41 c7 47 20 73 65 72 	movl   $0x76726573,0x20(%r15)
    1dc2:	76 
    1dc3:	66 41 c7 47 24 65 72 	movw   $0x7265,0x24(%r15)
    1dca:	41 c6 47 26 00       	movb   $0x0,0x26(%r15)
    1dcf:	44 89 e7             	mov    %r12d,%edi
    1dd2:	e8 99 f0 ff ff       	callq  e70 <close@plt>
    1dd7:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    1ddc:	e9 d2 03 00 00       	jmpq   21b3 <submitr+0x690>
    1de1:	48 b8 45 72 72 6f 72 	movabs $0x52203a726f727245,%rax
    1de8:	3a 20 52 
    1deb:	48 ba 65 73 75 6c 74 	movabs $0x747320746c757365,%rdx
    1df2:	20 73 74 
    1df5:	49 89 07             	mov    %rax,(%r15)
    1df8:	49 89 57 08          	mov    %rdx,0x8(%r15)
    1dfc:	48 b8 72 69 6e 67 20 	movabs $0x6f6f7420676e6972,%rax
    1e03:	74 6f 6f 
    1e06:	48 ba 20 6c 61 72 67 	movabs $0x202e656772616c20,%rdx
    1e0d:	65 2e 20 
    1e10:	49 89 47 10          	mov    %rax,0x10(%r15)
    1e14:	49 89 57 18          	mov    %rdx,0x18(%r15)
    1e18:	48 b8 49 6e 63 72 65 	movabs $0x6573616572636e49,%rax
    1e1f:	61 73 65 
    1e22:	48 ba 20 53 55 42 4d 	movabs $0x5254494d42555320,%rdx
    1e29:	49 54 52 
    1e2c:	49 89 47 20          	mov    %rax,0x20(%r15)
    1e30:	49 89 57 28          	mov    %rdx,0x28(%r15)
    1e34:	48 b8 5f 4d 41 58 42 	movabs $0x46554258414d5f,%rax
    1e3b:	55 46 00 
    1e3e:	49 89 47 30          	mov    %rax,0x30(%r15)
    1e42:	44 89 e7             	mov    %r12d,%edi
    1e45:	e8 26 f0 ff ff       	callq  e70 <close@plt>
    1e4a:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    1e4f:	e9 5f 03 00 00       	jmpq   21b3 <submitr+0x690>
    1e54:	49 0f a3 c5          	bt     %rax,%r13
    1e58:	73 21                	jae    1e7b <submitr+0x358>
    1e5a:	44 88 45 00          	mov    %r8b,0x0(%rbp)
    1e5e:	48 8d 6d 01          	lea    0x1(%rbp),%rbp
    1e62:	48 83 c3 01          	add    $0x1,%rbx
    1e66:	4c 39 f3             	cmp    %r14,%rbx
    1e69:	0f 84 6d 04 00 00    	je     22dc <submitr+0x7b9>
    1e6f:	44 0f b6 03          	movzbl (%rbx),%r8d
    1e73:	41 8d 40 d6          	lea    -0x2a(%r8),%eax
    1e77:	3c 35                	cmp    $0x35,%al
    1e79:	76 d9                	jbe    1e54 <submitr+0x331>
    1e7b:	44 89 c0             	mov    %r8d,%eax
    1e7e:	83 e0 df             	and    $0xffffffdf,%eax
    1e81:	83 e8 41             	sub    $0x41,%eax
    1e84:	3c 19                	cmp    $0x19,%al
    1e86:	76 d2                	jbe    1e5a <submitr+0x337>
    1e88:	41 80 f8 20          	cmp    $0x20,%r8b
    1e8c:	74 60                	je     1eee <submitr+0x3cb>
    1e8e:	41 8d 40 e0          	lea    -0x20(%r8),%eax
    1e92:	3c 5f                	cmp    $0x5f,%al
    1e94:	76 0a                	jbe    1ea0 <submitr+0x37d>
    1e96:	41 80 f8 09          	cmp    $0x9,%r8b
    1e9a:	0f 85 af 03 00 00    	jne    224f <submitr+0x72c>
    1ea0:	45 0f b6 c0          	movzbl %r8b,%r8d
    1ea4:	48 8d 0d bd 0c 00 00 	lea    0xcbd(%rip),%rcx        # 2b68 <array.3413+0x2e8>
    1eab:	ba 08 00 00 00       	mov    $0x8,%edx
    1eb0:	be 01 00 00 00       	mov    $0x1,%esi
    1eb5:	48 8b 7c 24 28       	mov    0x28(%rsp),%rdi
    1eba:	b8 00 00 00 00       	mov    $0x0,%eax
    1ebf:	e8 ac f0 ff ff       	callq  f70 <__sprintf_chk@plt>
    1ec4:	0f b6 84 24 60 80 00 	movzbl 0x8060(%rsp),%eax
    1ecb:	00 
    1ecc:	88 45 00             	mov    %al,0x0(%rbp)
    1ecf:	0f b6 84 24 61 80 00 	movzbl 0x8061(%rsp),%eax
    1ed6:	00 
    1ed7:	88 45 01             	mov    %al,0x1(%rbp)
    1eda:	0f b6 84 24 62 80 00 	movzbl 0x8062(%rsp),%eax
    1ee1:	00 
    1ee2:	88 45 02             	mov    %al,0x2(%rbp)
    1ee5:	48 8d 6d 03          	lea    0x3(%rbp),%rbp
    1ee9:	e9 74 ff ff ff       	jmpq   1e62 <submitr+0x33f>
    1eee:	c6 45 00 2b          	movb   $0x2b,0x0(%rbp)
    1ef2:	48 8d 6d 01          	lea    0x1(%rbp),%rbp
    1ef6:	e9 67 ff ff ff       	jmpq   1e62 <submitr+0x33f>
    1efb:	49 01 c5             	add    %rax,%r13
    1efe:	48 29 c5             	sub    %rax,%rbp
    1f01:	74 26                	je     1f29 <submitr+0x406>
    1f03:	48 89 ea             	mov    %rbp,%rdx
    1f06:	4c 89 ee             	mov    %r13,%rsi
    1f09:	44 89 e7             	mov    %r12d,%edi
    1f0c:	e8 2f ef ff ff       	callq  e40 <write@plt>
    1f11:	48 85 c0             	test   %rax,%rax
    1f14:	7f e5                	jg     1efb <submitr+0x3d8>
    1f16:	e8 f5 ee ff ff       	callq  e10 <__errno_location@plt>
    1f1b:	83 38 04             	cmpl   $0x4,(%rax)
    1f1e:	0f 85 31 01 00 00    	jne    2055 <submitr+0x532>
    1f24:	4c 89 f0             	mov    %r14,%rax
    1f27:	eb d2                	jmp    1efb <submitr+0x3d8>
    1f29:	48 85 db             	test   %rbx,%rbx
    1f2c:	0f 88 23 01 00 00    	js     2055 <submitr+0x532>
    1f32:	44 89 64 24 50       	mov    %r12d,0x50(%rsp)
    1f37:	c7 44 24 54 00 00 00 	movl   $0x0,0x54(%rsp)
    1f3e:	00 
    1f3f:	48 8d 7c 24 50       	lea    0x50(%rsp),%rdi
    1f44:	48 8d 47 10          	lea    0x10(%rdi),%rax
    1f48:	48 89 44 24 58       	mov    %rax,0x58(%rsp)
    1f4d:	48 8d b4 24 60 20 00 	lea    0x2060(%rsp),%rsi
    1f54:	00 
    1f55:	ba 00 20 00 00       	mov    $0x2000,%edx
    1f5a:	e8 08 fb ff ff       	callq  1a67 <rio_readlineb>
    1f5f:	48 85 c0             	test   %rax,%rax
    1f62:	0f 8e 4c 01 00 00    	jle    20b4 <submitr+0x591>
    1f68:	48 8d 4c 24 3c       	lea    0x3c(%rsp),%rcx
    1f6d:	48 8d 94 24 60 60 00 	lea    0x6060(%rsp),%rdx
    1f74:	00 
    1f75:	48 8d bc 24 60 20 00 	lea    0x2060(%rsp),%rdi
    1f7c:	00 
    1f7d:	4c 8d 84 24 60 80 00 	lea    0x8060(%rsp),%r8
    1f84:	00 
    1f85:	48 8d 35 e3 0b 00 00 	lea    0xbe3(%rip),%rsi        # 2b6f <array.3413+0x2ef>
    1f8c:	b8 00 00 00 00       	mov    $0x0,%eax
    1f91:	e8 5a ef ff ff       	callq  ef0 <__isoc99_sscanf@plt>
    1f96:	44 8b 44 24 3c       	mov    0x3c(%rsp),%r8d
    1f9b:	41 81 f8 c8 00 00 00 	cmp    $0xc8,%r8d
    1fa2:	0f 85 80 01 00 00    	jne    2128 <submitr+0x605>
    1fa8:	48 8d 9c 24 60 20 00 	lea    0x2060(%rsp),%rbx
    1faf:	00 
    1fb0:	48 8d 2d c9 0b 00 00 	lea    0xbc9(%rip),%rbp        # 2b80 <array.3413+0x300>
    1fb7:	4c 8d 6c 24 50       	lea    0x50(%rsp),%r13
    1fbc:	b9 03 00 00 00       	mov    $0x3,%ecx
    1fc1:	48 89 de             	mov    %rbx,%rsi
    1fc4:	48 89 ef             	mov    %rbp,%rdi
    1fc7:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
    1fc9:	0f 97 c0             	seta   %al
    1fcc:	1c 00                	sbb    $0x0,%al
    1fce:	84 c0                	test   %al,%al
    1fd0:	0f 84 89 01 00 00    	je     215f <submitr+0x63c>
    1fd6:	ba 00 20 00 00       	mov    $0x2000,%edx
    1fdb:	48 89 de             	mov    %rbx,%rsi
    1fde:	4c 89 ef             	mov    %r13,%rdi
    1fe1:	e8 81 fa ff ff       	callq  1a67 <rio_readlineb>
    1fe6:	48 85 c0             	test   %rax,%rax
    1fe9:	7f d1                	jg     1fbc <submitr+0x499>
    1feb:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    1ff2:	3a 20 43 
    1ff5:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    1ffc:	20 75 6e 
    1fff:	49 89 07             	mov    %rax,(%r15)
    2002:	49 89 57 08          	mov    %rdx,0x8(%r15)
    2006:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    200d:	74 6f 20 
    2010:	48 ba 72 65 61 64 20 	movabs $0x6165682064616572,%rdx
    2017:	68 65 61 
    201a:	49 89 47 10          	mov    %rax,0x10(%r15)
    201e:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2022:	48 b8 64 65 72 73 20 	movabs $0x6f72662073726564,%rax
    2029:	66 72 6f 
    202c:	48 ba 6d 20 73 65 72 	movabs $0x726576726573206d,%rdx
    2033:	76 65 72 
    2036:	49 89 47 20          	mov    %rax,0x20(%r15)
    203a:	49 89 57 28          	mov    %rdx,0x28(%r15)
    203e:	41 c6 47 30 00       	movb   $0x0,0x30(%r15)
    2043:	44 89 e7             	mov    %r12d,%edi
    2046:	e8 25 ee ff ff       	callq  e70 <close@plt>
    204b:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2050:	e9 5e 01 00 00       	jmpq   21b3 <submitr+0x690>
    2055:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    205c:	3a 20 43 
    205f:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    2066:	20 75 6e 
    2069:	49 89 07             	mov    %rax,(%r15)
    206c:	49 89 57 08          	mov    %rdx,0x8(%r15)
    2070:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    2077:	74 6f 20 
    207a:	48 ba 77 72 69 74 65 	movabs $0x6f74206574697277,%rdx
    2081:	20 74 6f 
    2084:	49 89 47 10          	mov    %rax,0x10(%r15)
    2088:	49 89 57 18          	mov    %rdx,0x18(%r15)
    208c:	48 b8 20 74 68 65 20 	movabs $0x7265732065687420,%rax
    2093:	73 65 72 
    2096:	49 89 47 20          	mov    %rax,0x20(%r15)
    209a:	41 c7 47 28 76 65 72 	movl   $0x726576,0x28(%r15)
    20a1:	00 
    20a2:	44 89 e7             	mov    %r12d,%edi
    20a5:	e8 c6 ed ff ff       	callq  e70 <close@plt>
    20aa:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    20af:	e9 ff 00 00 00       	jmpq   21b3 <submitr+0x690>
    20b4:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    20bb:	3a 20 43 
    20be:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    20c5:	20 75 6e 
    20c8:	49 89 07             	mov    %rax,(%r15)
    20cb:	49 89 57 08          	mov    %rdx,0x8(%r15)
    20cf:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    20d6:	74 6f 20 
    20d9:	48 ba 72 65 61 64 20 	movabs $0x7269662064616572,%rdx
    20e0:	66 69 72 
    20e3:	49 89 47 10          	mov    %rax,0x10(%r15)
    20e7:	49 89 57 18          	mov    %rdx,0x18(%r15)
    20eb:	48 b8 73 74 20 68 65 	movabs $0x6564616568207473,%rax
    20f2:	61 64 65 
    20f5:	48 ba 72 20 66 72 6f 	movabs $0x73206d6f72662072,%rdx
    20fc:	6d 20 73 
    20ff:	49 89 47 20          	mov    %rax,0x20(%r15)
    2103:	49 89 57 28          	mov    %rdx,0x28(%r15)
    2107:	41 c7 47 30 65 72 76 	movl   $0x65767265,0x30(%r15)
    210e:	65 
    210f:	66 41 c7 47 34 72 00 	movw   $0x72,0x34(%r15)
    2116:	44 89 e7             	mov    %r12d,%edi
    2119:	e8 52 ed ff ff       	callq  e70 <close@plt>
    211e:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2123:	e9 8b 00 00 00       	jmpq   21b3 <submitr+0x690>
    2128:	4c 8d 8c 24 60 80 00 	lea    0x8060(%rsp),%r9
    212f:	00 
    2130:	48 8d 0d 81 09 00 00 	lea    0x981(%rip),%rcx        # 2ab8 <array.3413+0x238>
    2137:	48 c7 c2 ff ff ff ff 	mov    $0xffffffffffffffff,%rdx
    213e:	be 01 00 00 00       	mov    $0x1,%esi
    2143:	4c 89 ff             	mov    %r15,%rdi
    2146:	b8 00 00 00 00       	mov    $0x0,%eax
    214b:	e8 20 ee ff ff       	callq  f70 <__sprintf_chk@plt>
    2150:	44 89 e7             	mov    %r12d,%edi
    2153:	e8 18 ed ff ff       	callq  e70 <close@plt>
    2158:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    215d:	eb 54                	jmp    21b3 <submitr+0x690>
    215f:	48 8d b4 24 60 20 00 	lea    0x2060(%rsp),%rsi
    2166:	00 
    2167:	48 8d 7c 24 50       	lea    0x50(%rsp),%rdi
    216c:	ba 00 20 00 00       	mov    $0x2000,%edx
    2171:	e8 f1 f8 ff ff       	callq  1a67 <rio_readlineb>
    2176:	48 85 c0             	test   %rax,%rax
    2179:	7e 61                	jle    21dc <submitr+0x6b9>
    217b:	48 8d b4 24 60 20 00 	lea    0x2060(%rsp),%rsi
    2182:	00 
    2183:	4c 89 ff             	mov    %r15,%rdi
    2186:	e8 95 ec ff ff       	callq  e20 <strcpy@plt>
    218b:	44 89 e7             	mov    %r12d,%edi
    218e:	e8 dd ec ff ff       	callq  e70 <close@plt>
    2193:	b9 03 00 00 00       	mov    $0x3,%ecx
    2198:	48 8d 3d e4 09 00 00 	lea    0x9e4(%rip),%rdi        # 2b83 <array.3413+0x303>
    219f:	4c 89 fe             	mov    %r15,%rsi
    21a2:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
    21a4:	0f 97 c0             	seta   %al
    21a7:	1c 00                	sbb    $0x0,%al
    21a9:	84 c0                	test   %al,%al
    21ab:	0f 95 c0             	setne  %al
    21ae:	0f b6 c0             	movzbl %al,%eax
    21b1:	f7 d8                	neg    %eax
    21b3:	48 8b 94 24 68 a0 00 	mov    0xa068(%rsp),%rdx
    21ba:	00 
    21bb:	64 48 33 14 25 28 00 	xor    %fs:0x28,%rdx
    21c2:	00 00 
    21c4:	0f 85 95 01 00 00    	jne    235f <submitr+0x83c>
    21ca:	48 81 c4 78 a0 00 00 	add    $0xa078,%rsp
    21d1:	5b                   	pop    %rbx
    21d2:	5d                   	pop    %rbp
    21d3:	41 5c                	pop    %r12
    21d5:	41 5d                	pop    %r13
    21d7:	41 5e                	pop    %r14
    21d9:	41 5f                	pop    %r15
    21db:	c3                   	retq   
    21dc:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    21e3:	3a 20 43 
    21e6:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    21ed:	20 75 6e 
    21f0:	49 89 07             	mov    %rax,(%r15)
    21f3:	49 89 57 08          	mov    %rdx,0x8(%r15)
    21f7:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    21fe:	74 6f 20 
    2201:	48 ba 72 65 61 64 20 	movabs $0x6174732064616572,%rdx
    2208:	73 74 61 
    220b:	49 89 47 10          	mov    %rax,0x10(%r15)
    220f:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2213:	48 b8 74 75 73 20 6d 	movabs $0x7373656d20737574,%rax
    221a:	65 73 73 
    221d:	48 ba 61 67 65 20 66 	movabs $0x6d6f726620656761,%rdx
    2224:	72 6f 6d 
    2227:	49 89 47 20          	mov    %rax,0x20(%r15)
    222b:	49 89 57 28          	mov    %rdx,0x28(%r15)
    222f:	48 b8 20 73 65 72 76 	movabs $0x72657672657320,%rax
    2236:	65 72 00 
    2239:	49 89 47 30          	mov    %rax,0x30(%r15)
    223d:	44 89 e7             	mov    %r12d,%edi
    2240:	e8 2b ec ff ff       	callq  e70 <close@plt>
    2245:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    224a:	e9 64 ff ff ff       	jmpq   21b3 <submitr+0x690>
    224f:	48 b8 45 72 72 6f 72 	movabs $0x52203a726f727245,%rax
    2256:	3a 20 52 
    2259:	48 ba 65 73 75 6c 74 	movabs $0x747320746c757365,%rdx
    2260:	20 73 74 
    2263:	49 89 07             	mov    %rax,(%r15)
    2266:	49 89 57 08          	mov    %rdx,0x8(%r15)
    226a:	48 b8 72 69 6e 67 20 	movabs $0x6e6f6320676e6972,%rax
    2271:	63 6f 6e 
    2274:	48 ba 74 61 69 6e 73 	movabs $0x6e6120736e696174,%rdx
    227b:	20 61 6e 
    227e:	49 89 47 10          	mov    %rax,0x10(%r15)
    2282:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2286:	48 b8 20 69 6c 6c 65 	movabs $0x6c6167656c6c6920,%rax
    228d:	67 61 6c 
    2290:	48 ba 20 6f 72 20 75 	movabs $0x72706e7520726f20,%rdx
    2297:	6e 70 72 
    229a:	49 89 47 20          	mov    %rax,0x20(%r15)
    229e:	49 89 57 28          	mov    %rdx,0x28(%r15)
    22a2:	48 b8 69 6e 74 61 62 	movabs $0x20656c6261746e69,%rax
    22a9:	6c 65 20 
    22ac:	48 ba 63 68 61 72 61 	movabs $0x6574636172616863,%rdx
    22b3:	63 74 65 
    22b6:	49 89 47 30          	mov    %rax,0x30(%r15)
    22ba:	49 89 57 38          	mov    %rdx,0x38(%r15)
    22be:	66 41 c7 47 40 72 2e 	movw   $0x2e72,0x40(%r15)
    22c5:	41 c6 47 42 00       	movb   $0x0,0x42(%r15)
    22ca:	44 89 e7             	mov    %r12d,%edi
    22cd:	e8 9e eb ff ff       	callq  e70 <close@plt>
    22d2:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    22d7:	e9 d7 fe ff ff       	jmpq   21b3 <submitr+0x690>
    22dc:	48 8d 9c 24 60 20 00 	lea    0x2060(%rsp),%rbx
    22e3:	00 
    22e4:	48 83 ec 08          	sub    $0x8,%rsp
    22e8:	48 8d 84 24 68 40 00 	lea    0x4068(%rsp),%rax
    22ef:	00 
    22f0:	50                   	push   %rax
    22f1:	ff 74 24 28          	pushq  0x28(%rsp)
    22f5:	ff 74 24 38          	pushq  0x38(%rsp)
    22f9:	4c 8b 4c 24 30       	mov    0x30(%rsp),%r9
    22fe:	4c 8b 44 24 28       	mov    0x28(%rsp),%r8
    2303:	48 8d 0d de 07 00 00 	lea    0x7de(%rip),%rcx        # 2ae8 <array.3413+0x268>
    230a:	ba 00 20 00 00       	mov    $0x2000,%edx
    230f:	be 01 00 00 00       	mov    $0x1,%esi
    2314:	48 89 df             	mov    %rbx,%rdi
    2317:	b8 00 00 00 00       	mov    $0x0,%eax
    231c:	e8 4f ec ff ff       	callq  f70 <__sprintf_chk@plt>
    2321:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
    2328:	b8 00 00 00 00       	mov    $0x0,%eax
    232d:	48 89 df             	mov    %rbx,%rdi
    2330:	f2 ae                	repnz scas %es:(%rdi),%al
    2332:	48 f7 d1             	not    %rcx
    2335:	48 89 cb             	mov    %rcx,%rbx
    2338:	48 83 eb 01          	sub    $0x1,%rbx
    233c:	48 83 c4 20          	add    $0x20,%rsp
    2340:	48 89 dd             	mov    %rbx,%rbp
    2343:	4c 8d ac 24 60 20 00 	lea    0x2060(%rsp),%r13
    234a:	00 
    234b:	41 be 00 00 00 00    	mov    $0x0,%r14d
    2351:	48 85 db             	test   %rbx,%rbx
    2354:	0f 85 a9 fb ff ff    	jne    1f03 <submitr+0x3e0>
    235a:	e9 d3 fb ff ff       	jmpq   1f32 <submitr+0x40f>
    235f:	e8 ec ea ff ff       	callq  e50 <__stack_chk_fail@plt>

0000000000002364 <init_timeout>:
    2364:	85 ff                	test   %edi,%edi
    2366:	74 25                	je     238d <init_timeout+0x29>
    2368:	53                   	push   %rbx
    2369:	89 fb                	mov    %edi,%ebx
    236b:	48 8d 35 c5 f6 ff ff 	lea    -0x93b(%rip),%rsi        # 1a37 <sigalrm_handler>
    2372:	bf 0e 00 00 00       	mov    $0xe,%edi
    2377:	e8 24 eb ff ff       	callq  ea0 <signal@plt>
    237c:	85 db                	test   %ebx,%ebx
    237e:	bf 00 00 00 00       	mov    $0x0,%edi
    2383:	0f 49 fb             	cmovns %ebx,%edi
    2386:	e8 d5 ea ff ff       	callq  e60 <alarm@plt>
    238b:	5b                   	pop    %rbx
    238c:	c3                   	retq   
    238d:	f3 c3                	repz retq 

000000000000238f <init_driver>:
    238f:	41 54                	push   %r12
    2391:	55                   	push   %rbp
    2392:	53                   	push   %rbx
    2393:	48 83 ec 20          	sub    $0x20,%rsp
    2397:	49 89 fc             	mov    %rdi,%r12
    239a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    23a1:	00 00 
    23a3:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
    23a8:	31 c0                	xor    %eax,%eax
    23aa:	be 01 00 00 00       	mov    $0x1,%esi
    23af:	bf 0d 00 00 00       	mov    $0xd,%edi
    23b4:	e8 e7 ea ff ff       	callq  ea0 <signal@plt>
    23b9:	be 01 00 00 00       	mov    $0x1,%esi
    23be:	bf 1d 00 00 00       	mov    $0x1d,%edi
    23c3:	e8 d8 ea ff ff       	callq  ea0 <signal@plt>
    23c8:	be 01 00 00 00       	mov    $0x1,%esi
    23cd:	bf 1d 00 00 00       	mov    $0x1d,%edi
    23d2:	e8 c9 ea ff ff       	callq  ea0 <signal@plt>
    23d7:	ba 00 00 00 00       	mov    $0x0,%edx
    23dc:	be 01 00 00 00       	mov    $0x1,%esi
    23e1:	bf 02 00 00 00       	mov    $0x2,%edi
    23e6:	e8 95 eb ff ff       	callq  f80 <socket@plt>
    23eb:	85 c0                	test   %eax,%eax
    23ed:	0f 88 a3 00 00 00    	js     2496 <init_driver+0x107>
    23f3:	89 c3                	mov    %eax,%ebx
    23f5:	48 8d 3d 8a 07 00 00 	lea    0x78a(%rip),%rdi        # 2b86 <array.3413+0x306>
    23fc:	e8 af ea ff ff       	callq  eb0 <gethostbyname@plt>
    2401:	48 85 c0             	test   %rax,%rax
    2404:	0f 84 df 00 00 00    	je     24e9 <init_driver+0x15a>
    240a:	48 89 e5             	mov    %rsp,%rbp
    240d:	48 c7 44 24 02 00 00 	movq   $0x0,0x2(%rsp)
    2414:	00 00 
    2416:	c7 45 0a 00 00 00 00 	movl   $0x0,0xa(%rbp)
    241d:	66 c7 45 0e 00 00    	movw   $0x0,0xe(%rbp)
    2423:	66 c7 04 24 02 00    	movw   $0x2,(%rsp)
    2429:	48 63 50 14          	movslq 0x14(%rax),%rdx
    242d:	48 8b 40 18          	mov    0x18(%rax),%rax
    2431:	48 8d 7d 04          	lea    0x4(%rbp),%rdi
    2435:	b9 0c 00 00 00       	mov    $0xc,%ecx
    243a:	48 8b 30             	mov    (%rax),%rsi
    243d:	e8 7e ea ff ff       	callq  ec0 <__memmove_chk@plt>
    2442:	66 c7 44 24 02 3b 6e 	movw   $0x6e3b,0x2(%rsp)
    2449:	ba 10 00 00 00       	mov    $0x10,%edx
    244e:	48 89 ee             	mov    %rbp,%rsi
    2451:	89 df                	mov    %ebx,%edi
    2453:	e8 d8 ea ff ff       	callq  f30 <connect@plt>
    2458:	85 c0                	test   %eax,%eax
    245a:	0f 88 fb 00 00 00    	js     255b <init_driver+0x1cc>
    2460:	89 df                	mov    %ebx,%edi
    2462:	e8 09 ea ff ff       	callq  e70 <close@plt>
    2467:	66 41 c7 04 24 4f 4b 	movw   $0x4b4f,(%r12)
    246e:	41 c6 44 24 02 00    	movb   $0x0,0x2(%r12)
    2474:	b8 00 00 00 00       	mov    $0x0,%eax
    2479:	48 8b 4c 24 18       	mov    0x18(%rsp),%rcx
    247e:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
    2485:	00 00 
    2487:	0f 85 06 01 00 00    	jne    2593 <init_driver+0x204>
    248d:	48 83 c4 20          	add    $0x20,%rsp
    2491:	5b                   	pop    %rbx
    2492:	5d                   	pop    %rbp
    2493:	41 5c                	pop    %r12
    2495:	c3                   	retq   
    2496:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    249d:	3a 20 43 
    24a0:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    24a7:	20 75 6e 
    24aa:	49 89 04 24          	mov    %rax,(%r12)
    24ae:	49 89 54 24 08       	mov    %rdx,0x8(%r12)
    24b3:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    24ba:	74 6f 20 
    24bd:	48 ba 63 72 65 61 74 	movabs $0x7320657461657263,%rdx
    24c4:	65 20 73 
    24c7:	49 89 44 24 10       	mov    %rax,0x10(%r12)
    24cc:	49 89 54 24 18       	mov    %rdx,0x18(%r12)
    24d1:	41 c7 44 24 20 6f 63 	movl   $0x656b636f,0x20(%r12)
    24d8:	6b 65 
    24da:	66 41 c7 44 24 24 74 	movw   $0x74,0x24(%r12)
    24e1:	00 
    24e2:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    24e7:	eb 90                	jmp    2479 <init_driver+0xea>
    24e9:	48 b8 45 72 72 6f 72 	movabs $0x44203a726f727245,%rax
    24f0:	3a 20 44 
    24f3:	48 ba 4e 53 20 69 73 	movabs $0x6e7520736920534e,%rdx
    24fa:	20 75 6e 
    24fd:	49 89 04 24          	mov    %rax,(%r12)
    2501:	49 89 54 24 08       	mov    %rdx,0x8(%r12)
    2506:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    250d:	74 6f 20 
    2510:	48 ba 72 65 73 6f 6c 	movabs $0x2065766c6f736572,%rdx
    2517:	76 65 20 
    251a:	49 89 44 24 10       	mov    %rax,0x10(%r12)
    251f:	49 89 54 24 18       	mov    %rdx,0x18(%r12)
    2524:	48 b8 73 65 72 76 65 	movabs $0x6120726576726573,%rax
    252b:	72 20 61 
    252e:	49 89 44 24 20       	mov    %rax,0x20(%r12)
    2533:	41 c7 44 24 28 64 64 	movl   $0x65726464,0x28(%r12)
    253a:	72 65 
    253c:	66 41 c7 44 24 2c 73 	movw   $0x7373,0x2c(%r12)
    2543:	73 
    2544:	41 c6 44 24 2e 00    	movb   $0x0,0x2e(%r12)
    254a:	89 df                	mov    %ebx,%edi
    254c:	e8 1f e9 ff ff       	callq  e70 <close@plt>
    2551:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2556:	e9 1e ff ff ff       	jmpq   2479 <init_driver+0xea>
    255b:	4c 8d 05 24 06 00 00 	lea    0x624(%rip),%r8        # 2b86 <array.3413+0x306>
    2562:	48 8d 0d d7 05 00 00 	lea    0x5d7(%rip),%rcx        # 2b40 <array.3413+0x2c0>
    2569:	48 c7 c2 ff ff ff ff 	mov    $0xffffffffffffffff,%rdx
    2570:	be 01 00 00 00       	mov    $0x1,%esi
    2575:	4c 89 e7             	mov    %r12,%rdi
    2578:	b8 00 00 00 00       	mov    $0x0,%eax
    257d:	e8 ee e9 ff ff       	callq  f70 <__sprintf_chk@plt>
    2582:	89 df                	mov    %ebx,%edi
    2584:	e8 e7 e8 ff ff       	callq  e70 <close@plt>
    2589:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    258e:	e9 e6 fe ff ff       	jmpq   2479 <init_driver+0xea>
    2593:	e8 b8 e8 ff ff       	callq  e50 <__stack_chk_fail@plt>

0000000000002598 <driver_post>:
    2598:	53                   	push   %rbx
    2599:	4c 89 c3             	mov    %r8,%rbx
    259c:	85 c9                	test   %ecx,%ecx
    259e:	75 17                	jne    25b7 <driver_post+0x1f>
    25a0:	48 85 ff             	test   %rdi,%rdi
    25a3:	74 05                	je     25aa <driver_post+0x12>
    25a5:	80 3f 00             	cmpb   $0x0,(%rdi)
    25a8:	75 33                	jne    25dd <driver_post+0x45>
    25aa:	66 c7 03 4f 4b       	movw   $0x4b4f,(%rbx)
    25af:	c6 43 02 00          	movb   $0x0,0x2(%rbx)
    25b3:	89 c8                	mov    %ecx,%eax
    25b5:	5b                   	pop    %rbx
    25b6:	c3                   	retq   
    25b7:	48 8d 35 e0 05 00 00 	lea    0x5e0(%rip),%rsi        # 2b9e <array.3413+0x31e>
    25be:	bf 01 00 00 00       	mov    $0x1,%edi
    25c3:	b8 00 00 00 00       	mov    $0x0,%eax
    25c8:	e8 33 e9 ff ff       	callq  f00 <__printf_chk@plt>
    25cd:	66 c7 03 4f 4b       	movw   $0x4b4f,(%rbx)
    25d2:	c6 43 02 00          	movb   $0x0,0x2(%rbx)
    25d6:	b8 00 00 00 00       	mov    $0x0,%eax
    25db:	eb d8                	jmp    25b5 <driver_post+0x1d>
    25dd:	41 50                	push   %r8
    25df:	52                   	push   %rdx
    25e0:	4c 8d 0d 9b 05 00 00 	lea    0x59b(%rip),%r9        # 2b82 <array.3413+0x302>
    25e7:	49 89 f0             	mov    %rsi,%r8
    25ea:	48 89 f9             	mov    %rdi,%rcx
    25ed:	48 8d 15 c1 05 00 00 	lea    0x5c1(%rip),%rdx        # 2bb5 <array.3413+0x335>
    25f4:	be 6e 3b 00 00       	mov    $0x3b6e,%esi
    25f9:	48 8d 3d 86 05 00 00 	lea    0x586(%rip),%rdi        # 2b86 <array.3413+0x306>
    2600:	e8 1e f5 ff ff       	callq  1b23 <submitr>
    2605:	48 83 c4 10          	add    $0x10,%rsp
    2609:	eb aa                	jmp    25b5 <driver_post+0x1d>
    260b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000002610 <__libc_csu_init>:
    2610:	41 57                	push   %r15
    2612:	41 56                	push   %r14
    2614:	49 89 d7             	mov    %rdx,%r15
    2617:	41 55                	push   %r13
    2619:	41 54                	push   %r12
    261b:	4c 8d 25 d6 16 20 00 	lea    0x2016d6(%rip),%r12        # 203cf8 <__frame_dummy_init_array_entry>
    2622:	55                   	push   %rbp
    2623:	48 8d 2d d6 16 20 00 	lea    0x2016d6(%rip),%rbp        # 203d00 <__init_array_end>
    262a:	53                   	push   %rbx
    262b:	41 89 fd             	mov    %edi,%r13d
    262e:	49 89 f6             	mov    %rsi,%r14
    2631:	4c 29 e5             	sub    %r12,%rbp
    2634:	48 83 ec 08          	sub    $0x8,%rsp
    2638:	48 c1 fd 03          	sar    $0x3,%rbp
    263c:	e8 8f e7 ff ff       	callq  dd0 <_init>
    2641:	48 85 ed             	test   %rbp,%rbp
    2644:	74 20                	je     2666 <__libc_csu_init+0x56>
    2646:	31 db                	xor    %ebx,%ebx
    2648:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
    264f:	00 
    2650:	4c 89 fa             	mov    %r15,%rdx
    2653:	4c 89 f6             	mov    %r14,%rsi
    2656:	44 89 ef             	mov    %r13d,%edi
    2659:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
    265d:	48 83 c3 01          	add    $0x1,%rbx
    2661:	48 39 dd             	cmp    %rbx,%rbp
    2664:	75 ea                	jne    2650 <__libc_csu_init+0x40>
    2666:	48 83 c4 08          	add    $0x8,%rsp
    266a:	5b                   	pop    %rbx
    266b:	5d                   	pop    %rbp
    266c:	41 5c                	pop    %r12
    266e:	41 5d                	pop    %r13
    2670:	41 5e                	pop    %r14
    2672:	41 5f                	pop    %r15
    2674:	c3                   	retq   
    2675:	90                   	nop
    2676:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    267d:	00 00 00 

0000000000002680 <__libc_csu_fini>:
    2680:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000002684 <_fini>:
    2684:	48 83 ec 08          	sub    $0x8,%rsp
    2688:	48 83 c4 08          	add    $0x8,%rsp
    268c:	c3                   	retq   

Disassembly of section .rodata:

00000000000026a0 <_IO_stdin_used>:
    26a0:	01 00                	add    %eax,(%rax)
    26a2:	02 00                	add    (%rax),%al
    26a4:	72 00                	jb     26a6 <_IO_stdin_used+0x6>
    26a6:	25 73 3a 20 45       	and    $0x45203a73,%eax
    26ab:	72 72                	jb     271f <_IO_stdin_used+0x7f>
    26ad:	6f                   	outsl  %ds:(%rsi),(%dx)
    26ae:	72 3a                	jb     26ea <_IO_stdin_used+0x4a>
    26b0:	20 43 6f             	and    %al,0x6f(%rbx)
    26b3:	75 6c                	jne    2721 <_IO_stdin_used+0x81>
    26b5:	64 6e                	outsb  %fs:(%rsi),(%dx)
    26b7:	27                   	(bad)  
    26b8:	74 20                	je     26da <_IO_stdin_used+0x3a>
    26ba:	6f                   	outsl  %ds:(%rsi),(%dx)
    26bb:	70 65                	jo     2722 <_IO_stdin_used+0x82>
    26bd:	6e                   	outsb  %ds:(%rsi),(%dx)
    26be:	20 25 73 0a 00 55    	and    %ah,0x55000a73(%rip)        # 55003137 <_end+0x54dfe407>
    26c4:	73 61                	jae    2727 <_IO_stdin_used+0x87>
    26c6:	67 65 3a 20          	cmp    %gs:(%eax),%ah
    26ca:	25 73 20 5b 3c       	and    $0x3c5b2073,%eax
    26cf:	69 6e 70 75 74 5f 66 	imul   $0x665f7475,0x70(%rsi),%ebp
    26d6:	69 6c 65 3e 5d 0a 00 	imul   $0x54000a5d,0x3e(%rbp,%riz,2),%ebp
    26dd:	54 
    26de:	68 61 74 27 73       	pushq  $0x73277461
    26e3:	20 6e 75             	and    %ch,0x75(%rsi)
    26e6:	6d                   	insl   (%dx),%es:(%rdi)
    26e7:	62                   	(bad)  
    26e8:	65 72 20             	gs jb  270b <_IO_stdin_used+0x6b>
    26eb:	32 2e                	xor    (%rsi),%ch
    26ed:	20 20                	and    %ah,(%rax)
    26ef:	4b                   	rex.WXB
    26f0:	65 65 70 20          	gs gs jo 2714 <_IO_stdin_used+0x74>
    26f4:	67 6f                	outsl  %ds:(%esi),(%dx)
    26f6:	69 6e 67 21 00 48 61 	imul   $0x61480021,0x67(%rsi),%ebp
    26fd:	6c                   	insb   (%dx),%es:(%rdi)
    26fe:	66 77 61             	data16 ja 2762 <_IO_stdin_used+0xc2>
    2701:	79 20                	jns    2723 <_IO_stdin_used+0x83>
    2703:	74 68                	je     276d <_IO_stdin_used+0xcd>
    2705:	65 72 65             	gs jb  276d <_IO_stdin_used+0xcd>
    2708:	21 00                	and    %eax,(%rax)
    270a:	47 6f                	rex.RXB outsl %ds:(%rsi),(%dx)
    270c:	6f                   	outsl  %ds:(%rsi),(%dx)
    270d:	64 20 77 6f          	and    %dh,%fs:0x6f(%rdi)
    2711:	72 6b                	jb     277e <_IO_stdin_used+0xde>
    2713:	21 20                	and    %esp,(%rax)
    2715:	20 4f 6e             	and    %cl,0x6e(%rdi)
    2718:	20 74 6f 20          	and    %dh,0x20(%rdi,%rbp,2)
    271c:	74 68                	je     2786 <_IO_stdin_used+0xe6>
    271e:	65 20 6e 65          	and    %ch,%gs:0x65(%rsi)
    2722:	78 74                	js     2798 <_IO_stdin_used+0xf8>
    2724:	2e 2e 2e 00 57 65    	cs cs add %dl,%cs:0x65(%rdi)
    272a:	6c                   	insb   (%dx),%es:(%rdi)
    272b:	63 6f 6d             	movslq 0x6d(%rdi),%ebp
    272e:	65 20 74 6f 20       	and    %dh,%gs:0x20(%rdi,%rbp,2)
    2733:	6d                   	insl   (%dx),%es:(%rdi)
    2734:	79 20                	jns    2756 <_IO_stdin_used+0xb6>
    2736:	66 69 65 6e 64 69    	imul   $0x6964,0x6e(%rbp),%sp
    273c:	73 68                	jae    27a6 <_IO_stdin_used+0x106>
    273e:	20 6c 69 74          	and    %ch,0x74(%rcx,%rbp,2)
    2742:	74 6c                	je     27b0 <_IO_stdin_used+0x110>
    2744:	65 20 62 6f          	and    %ah,%gs:0x6f(%rdx)
    2748:	6d                   	insl   (%dx),%es:(%rdi)
    2749:	62                   	(bad)  
    274a:	2e 20 59 6f          	and    %bl,%cs:0x6f(%rcx)
    274e:	75 20                	jne    2770 <_IO_stdin_used+0xd0>
    2750:	68 61 76 65 20       	pushq  $0x20657661
    2755:	36 20 70 68          	and    %dh,%ss:0x68(%rax)
    2759:	61                   	(bad)  
    275a:	73 65                	jae    27c1 <_IO_stdin_used+0x121>
    275c:	73 20                	jae    277e <_IO_stdin_used+0xde>
    275e:	77 69                	ja     27c9 <_IO_stdin_used+0x129>
    2760:	74 68                	je     27ca <_IO_stdin_used+0x12a>
    2762:	00 00                	add    %al,(%rax)
    2764:	00 00                	add    %al,(%rax)
    2766:	00 00                	add    %al,(%rax)
    2768:	77 68                	ja     27d2 <_IO_stdin_used+0x132>
    276a:	69 63 68 20 74 6f 20 	imul   $0x206f7420,0x68(%rbx),%esp
    2771:	62                   	(bad)  
    2772:	6c                   	insb   (%dx),%es:(%rdi)
    2773:	6f                   	outsl  %ds:(%rsi),(%dx)
    2774:	77 20                	ja     2796 <_IO_stdin_used+0xf6>
    2776:	79 6f                	jns    27e7 <_IO_stdin_used+0x147>
    2778:	75 72                	jne    27ec <_IO_stdin_used+0x14c>
    277a:	73 65                	jae    27e1 <_IO_stdin_used+0x141>
    277c:	6c                   	insb   (%dx),%es:(%rdi)
    277d:	66 20 75 70          	data16 and %dh,0x70(%rbp)
    2781:	2e 20 48 61          	and    %cl,%cs:0x61(%rax)
    2785:	76 65                	jbe    27ec <_IO_stdin_used+0x14c>
    2787:	20 61 20             	and    %ah,0x20(%rcx)
    278a:	6e                   	outsb  %ds:(%rsi),(%dx)
    278b:	69 63 65 20 64 61 79 	imul   $0x79616420,0x65(%rbx),%esp
    2792:	21 00                	and    %eax,(%rax)
    2794:	00 00                	add    %al,(%rax)
    2796:	00 00                	add    %al,(%rax)
    2798:	50                   	push   %rax
    2799:	68 61 73 65 20       	pushq  $0x20657361
    279e:	31 20                	xor    %esp,(%rax)
    27a0:	64 65 66 75 73       	fs gs data16 jne 2818 <_IO_stdin_used+0x178>
    27a5:	65 64 2e 20 48 6f    	gs fs and %cl,%cs:0x6f(%rax)
    27ab:	77 20                	ja     27cd <_IO_stdin_used+0x12d>
    27ad:	61                   	(bad)  
    27ae:	62                   	(bad)  
    27af:	6f                   	outsl  %ds:(%rsi),(%dx)
    27b0:	75 74                	jne    2826 <_IO_stdin_used+0x186>
    27b2:	20 74 68 65          	and    %dh,0x65(%rax,%rbp,2)
    27b6:	20 6e 65             	and    %ch,0x65(%rsi)
    27b9:	78 74                	js     282f <_IO_stdin_used+0x18f>
    27bb:	20 6f 6e             	and    %ch,0x6e(%rdi)
    27be:	65 3f                	gs (bad) 
	...
    27c8:	53                   	push   %rbx
    27c9:	6f                   	outsl  %ds:(%rsi),(%dx)
    27ca:	20 79 6f             	and    %bh,0x6f(%rcx)
    27cd:	75 20                	jne    27ef <_IO_stdin_used+0x14f>
    27cf:	67 6f                	outsl  %ds:(%esi),(%dx)
    27d1:	74 20                	je     27f3 <_IO_stdin_used+0x153>
    27d3:	74 68                	je     283d <_IO_stdin_used+0x19d>
    27d5:	61                   	(bad)  
    27d6:	74 20                	je     27f8 <_IO_stdin_used+0x158>
    27d8:	6f                   	outsl  %ds:(%rsi),(%dx)
    27d9:	6e                   	outsb  %ds:(%rsi),(%dx)
    27da:	65 2e 20 20          	gs and %ah,%cs:(%rax)
    27de:	54                   	push   %rsp
    27df:	72 79                	jb     285a <_IO_stdin_used+0x1ba>
    27e1:	20 74 68 69          	and    %dh,0x69(%rax,%rbp,2)
    27e5:	73 20                	jae    2807 <_IO_stdin_used+0x167>
    27e7:	6f                   	outsl  %ds:(%rsi),(%dx)
    27e8:	6e                   	outsb  %ds:(%rsi),(%dx)
    27e9:	65 2e 00 00          	gs add %al,%cs:(%rax)
    27ed:	00 00                	add    %al,(%rax)
    27ef:	00 57 68             	add    %dl,0x68(%rdi)
    27f2:	79 20                	jns    2814 <_IO_stdin_used+0x174>
    27f4:	6d                   	insl   (%dx),%es:(%rdi)
    27f5:	61                   	(bad)  
    27f6:	6b 65 20 74          	imul   $0x74,0x20(%rbp),%esp
    27fa:	72 69                	jb     2865 <_IO_stdin_used+0x1c5>
    27fc:	6c                   	insb   (%dx),%es:(%rdi)
    27fd:	6c                   	insb   (%dx),%es:(%rdi)
    27fe:	69 6f 6e 73 20 77 68 	imul   $0x68772073,0x6e(%rdi),%ebp
    2805:	65 6e                	outsb  %gs:(%rsi),(%dx)
    2807:	20 77 65             	and    %dh,0x65(%rdi)
    280a:	20 63 6f             	and    %ah,0x6f(%rbx)
    280d:	75 6c                	jne    287b <_IO_stdin_used+0x1db>
    280f:	64 20 6d 61          	and    %ch,%fs:0x61(%rbp)
    2813:	6b 65 2e 2e          	imul   $0x2e,0x2e(%rbp),%esp
    2817:	2e 20 62 69          	and    %ah,%cs:0x69(%rdx)
    281b:	6c                   	insb   (%dx),%es:(%rdi)
    281c:	6c                   	insb   (%dx),%es:(%rdi)
    281d:	69 6f 6e 73 3f 00 00 	imul   $0x3f73,0x6e(%rdi),%ebp
    2824:	00 00                	add    %al,(%rax)
    2826:	00 00                	add    %al,(%rax)
    2828:	57                   	push   %rdi
    2829:	6f                   	outsl  %ds:(%rsi),(%dx)
    282a:	77 21                	ja     284d <_IO_stdin_used+0x1ad>
    282c:	20 59 6f             	and    %bl,0x6f(%rcx)
    282f:	75 27                	jne    2858 <_IO_stdin_used+0x1b8>
    2831:	76 65                	jbe    2898 <array.3413+0x18>
    2833:	20 64 65 66          	and    %ah,0x66(%rbp,%riz,2)
    2837:	75 73                	jne    28ac <array.3413+0x2c>
    2839:	65 64 20 74 68 65    	gs and %dh,%fs:0x65(%rax,%rbp,2)
    283f:	20 73 65             	and    %dh,0x65(%rbx)
    2842:	63 72 65             	movslq 0x65(%rdx),%esi
    2845:	74 20                	je     2867 <_IO_stdin_used+0x1c7>
    2847:	73 74                	jae    28bd <array.3413+0x3d>
    2849:	61                   	(bad)  
    284a:	67 65 21 00          	and    %eax,%gs:(%eax)
	...
    285e:	00 00                	add    %al,(%rax)
    2860:	bd ea ff ff 80       	mov    $0x80ffffea,%ebp
    2865:	ea                   	(bad)  
    2866:	ff                   	(bad)  
    2867:	ff 87 ea ff ff 8e    	incl   -0x71000016(%rdi)
    286d:	ea                   	(bad)  
    286e:	ff                   	(bad)  
    286f:	ff 95 ea ff ff 9c    	callq  *-0x63000016(%rbp)
    2875:	ea                   	(bad)  
    2876:	ff                   	(bad)  
    2877:	ff a3 ea ff ff aa    	jmpq   *-0x55000016(%rbx)
    287d:	ea                   	(bad)  
    287e:	ff                   	(bad)  
    287f:	ff                 	decl   (%rdx)

0000000000002880 <array.3413>:
    2880:	0a 00                	or     (%rax),%al
    2882:	00 00                	add    %al,(%rax)
    2884:	02 00                	add    (%rax),%al
    2886:	00 00                	add    %al,(%rax)
    2888:	0e                   	(bad)  
    2889:	00 00                	add    %al,(%rax)
    288b:	00 07                	add    %al,(%rdi)
    288d:	00 00                	add    %al,(%rax)
    288f:	00 08                	add    %cl,(%rax)
    2891:	00 00                	add    %al,(%rax)
    2893:	00 0c 00             	add    %cl,(%rax,%rax,1)
    2896:	00 00                	add    %al,(%rax)
    2898:	0f 00 00             	sldt   (%rax)
    289b:	00 0b                	add    %cl,(%rbx)
    289d:	00 00                	add    %al,(%rax)
    289f:	00 00                	add    %al,(%rax)
    28a1:	00 00                	add    %al,(%rax)
    28a3:	00 04 00             	add    %al,(%rax,%rax,1)
    28a6:	00 00                	add    %al,(%rax)
    28a8:	01 00                	add    %eax,(%rax)
    28aa:	00 00                	add    %al,(%rax)
    28ac:	0d 00 00 00 03       	or     $0x3000000,%eax
    28b1:	00 00                	add    %al,(%rax)
    28b3:	00 09                	add    %cl,(%rcx)
    28b5:	00 00                	add    %al,(%rax)
    28b7:	00 06                	add    %al,(%rsi)
    28b9:	00 00                	add    %al,(%rax)
    28bb:	00 05 00 00 00 53    	add    %al,0x53000000(%rip)        # 530028c1 <_end+0x52dfdb91>
    28c1:	6f                   	outsl  %ds:(%rsi),(%dx)
    28c2:	20 79 6f             	and    %bh,0x6f(%rcx)
    28c5:	75 20                	jne    28e7 <array.3413+0x67>
    28c7:	74 68                	je     2931 <array.3413+0xb1>
    28c9:	69 6e 6b 20 79 6f 75 	imul   $0x756f7920,0x6b(%rsi),%ebp
    28d0:	20 63 61             	and    %ah,0x61(%rbx)
    28d3:	6e                   	outsb  %ds:(%rsi),(%dx)
    28d4:	20 73 74             	and    %dh,0x74(%rbx)
    28d7:	6f                   	outsl  %ds:(%rsi),(%dx)
    28d8:	70 20                	jo     28fa <array.3413+0x7a>
    28da:	74 68                	je     2944 <array.3413+0xc4>
    28dc:	65 20 62 6f          	and    %ah,%gs:0x6f(%rdx)
    28e0:	6d                   	insl   (%dx),%es:(%rdi)
    28e1:	62                   	(bad)  
    28e2:	20 77 69             	and    %dh,0x69(%rdi)
    28e5:	74 68                	je     294f <array.3413+0xcf>
    28e7:	20 63 74             	and    %ah,0x74(%rbx)
    28ea:	72 6c                	jb     2958 <array.3413+0xd8>
    28ec:	2d 63 2c 20 64       	sub    $0x64202c63,%eax
    28f1:	6f                   	outsl  %ds:(%rsi),(%dx)
    28f2:	20 79 6f             	and    %bh,0x6f(%rcx)
    28f5:	75 3f                	jne    2936 <array.3413+0xb6>
    28f7:	00 43 75             	add    %al,0x75(%rbx)
    28fa:	72 73                	jb     296f <array.3413+0xef>
    28fc:	65 73 2c             	gs jae 292b <array.3413+0xab>
    28ff:	20 79 6f             	and    %bh,0x6f(%rcx)
    2902:	75 27                	jne    292b <array.3413+0xab>
    2904:	76 65                	jbe    296b <array.3413+0xeb>
    2906:	20 66 6f             	and    %ah,0x6f(%rsi)
    2909:	75 6e                	jne    2979 <array.3413+0xf9>
    290b:	64 20 74 68 65       	and    %dh,%fs:0x65(%rax,%rbp,2)
    2910:	20 73 65             	and    %dh,0x65(%rbx)
    2913:	63 72 65             	movslq 0x65(%rdx),%esi
    2916:	74 20                	je     2938 <array.3413+0xb8>
    2918:	70 68                	jo     2982 <array.3413+0x102>
    291a:	61                   	(bad)  
    291b:	73 65                	jae    2982 <array.3413+0x102>
    291d:	21 00                	and    %eax,(%rax)
    291f:	00 42 75             	add    %al,0x75(%rdx)
    2922:	74 20                	je     2944 <array.3413+0xc4>
    2924:	66 69 6e 64 69 6e    	imul   $0x6e69,0x64(%rsi),%bp
    292a:	67 20 69 74          	and    %ch,0x74(%ecx)
    292e:	20 61 6e             	and    %ah,0x6e(%rcx)
    2931:	64 20 73 6f          	and    %dh,%fs:0x6f(%rbx)
    2935:	6c                   	insb   (%dx),%es:(%rdi)
    2936:	76 69                	jbe    29a1 <array.3413+0x121>
    2938:	6e                   	outsb  %ds:(%rsi),(%dx)
    2939:	67 20 69 74          	and    %ch,0x74(%ecx)
    293d:	20 61 72             	and    %ah,0x72(%rcx)
    2940:	65 20 71 75          	and    %dh,%gs:0x75(%rcx)
    2944:	69 74 65 20 64 69 66 	imul   $0x66666964,0x20(%rbp,%riz,2),%esi
    294b:	66 
    294c:	65 72 65             	gs jb  29b4 <array.3413+0x134>
    294f:	6e                   	outsb  %ds:(%rsi),(%dx)
    2950:	74 2e                	je     2980 <array.3413+0x100>
    2952:	2e 2e 00 00          	cs add %al,%cs:(%rax)
    2956:	00 00                	add    %al,(%rax)
    2958:	43 6f                	rex.XB outsl %ds:(%rsi),(%dx)
    295a:	6e                   	outsb  %ds:(%rsi),(%dx)
    295b:	67 72 61             	addr32 jb 29bf <array.3413+0x13f>
    295e:	74 75                	je     29d5 <array.3413+0x155>
    2960:	6c                   	insb   (%dx),%es:(%rdi)
    2961:	61                   	(bad)  
    2962:	74 69                	je     29cd <array.3413+0x14d>
    2964:	6f                   	outsl  %ds:(%rsi),(%dx)
    2965:	6e                   	outsb  %ds:(%rsi),(%dx)
    2966:	73 21                	jae    2989 <array.3413+0x109>
    2968:	20 59 6f             	and    %bl,0x6f(%rcx)
    296b:	75 27                	jne    2994 <array.3413+0x114>
    296d:	76 65                	jbe    29d4 <array.3413+0x154>
    296f:	20 64 65 66          	and    %ah,0x66(%rbp,%riz,2)
    2973:	75 73                	jne    29e8 <array.3413+0x168>
    2975:	65 64 20 74 68 65    	gs and %dh,%fs:0x65(%rax,%rbp,2)
    297b:	20 62 6f             	and    %ah,0x6f(%rdx)
    297e:	6d                   	insl   (%dx),%es:(%rdi)
    297f:	62 21                	(bad)  
    2981:	00 57 65             	add    %dl,0x65(%rdi)
    2984:	6c                   	insb   (%dx),%es:(%rdi)
    2985:	6c                   	insb   (%dx),%es:(%rdi)
    2986:	2e 2e 2e 00 4f 4b    	cs cs add %cl,%cs:0x4b(%rdi)
    298c:	2e 20 3a             	and    %bh,%cs:(%rdx)
    298f:	2d 29 00 49 6e       	sub    $0x6e490029,%eax
    2994:	76 61                	jbe    29f7 <array.3413+0x177>
    2996:	6c                   	insb   (%dx),%es:(%rdi)
    2997:	69 64 20 70 68 61 73 	imul   $0x65736168,0x70(%rax,%riz,1),%esp
    299e:	65 
    299f:	25 73 0a 00 0a       	and    $0xa000a73,%eax
    29a4:	42                   	rex.X
    29a5:	4f                   	rex.WRXB
    29a6:	4f                   	rex.WRXB
    29a7:	4d 21 21             	and    %r12,(%r9)
    29aa:	21 00                	and    %eax,(%rax)
    29ac:	54                   	push   %rsp
    29ad:	68 65 20 62 6f       	pushq  $0x6f622065
    29b2:	6d                   	insl   (%dx),%es:(%rdi)
    29b3:	62                   	(bad)  
    29b4:	20 68 61             	and    %ch,0x61(%rax)
    29b7:	73 20                	jae    29d9 <array.3413+0x159>
    29b9:	62                   	(bad)  
    29ba:	6c                   	insb   (%dx),%es:(%rdi)
    29bb:	6f                   	outsl  %ds:(%rsi),(%dx)
    29bc:	77 6e                	ja     2a2c <array.3413+0x1ac>
    29be:	20 75 70             	and    %dh,0x70(%rbp)
    29c1:	2e 00 25 64 20 25 64 	add    %ah,%cs:0x64252064(%rip)        # 64254a2c <_end+0x6404fcfc>
    29c8:	20 25 64 20 25 64    	and    %ah,0x64252064(%rip)        # 64254a32 <_end+0x6404fd02>
    29ce:	20 25 64 20 25 64    	and    %ah,0x64252064(%rip)        # 64254a38 <_end+0x6404fd08>
    29d4:	00 45 72             	add    %al,0x72(%rbp)
    29d7:	72 6f                	jb     2a48 <array.3413+0x1c8>
    29d9:	72 3a                	jb     2a15 <array.3413+0x195>
    29db:	20 50 72             	and    %dl,0x72(%rax)
    29de:	65 6d                	gs insl (%dx),%es:(%rdi)
    29e0:	61                   	(bad)  
    29e1:	74 75                	je     2a58 <array.3413+0x1d8>
    29e3:	72 65                	jb     2a4a <array.3413+0x1ca>
    29e5:	20 45 4f             	and    %al,0x4f(%rbp)
    29e8:	46 20 6f 6e          	rex.RX and %r13b,0x6e(%rdi)
    29ec:	20 73 74             	and    %dh,0x74(%rbx)
    29ef:	64 69 6e 00 47 52 41 	imul   $0x44415247,%fs:0x0(%rsi),%ebp
    29f6:	44 
    29f7:	45 5f                	rex.RB pop %r15
    29f9:	42                   	rex.X
    29fa:	4f                   	rex.WRXB
    29fb:	4d                   	rex.WRB
    29fc:	42 00 45 72          	rex.X add %al,0x72(%rbp)
    2a00:	72 6f                	jb     2a71 <array.3413+0x1f1>
    2a02:	72 3a                	jb     2a3e <array.3413+0x1be>
    2a04:	20 49 6e             	and    %cl,0x6e(%rcx)
    2a07:	70 75                	jo     2a7e <array.3413+0x1fe>
    2a09:	74 20                	je     2a2b <array.3413+0x1ab>
    2a0b:	6c                   	insb   (%dx),%es:(%rdi)
    2a0c:	69 6e 65 20 74 6f 6f 	imul   $0x6f6f7420,0x65(%rsi),%ebp
    2a13:	20 6c 6f 6e          	and    %ch,0x6e(%rdi,%rbp,2)
    2a17:	67 00 25 64 20 25 64 	add    %ah,0x64252064(%eip)        # 64254a82 <_end+0x6404fd52>
    2a1e:	20 25 73 00 44 72    	and    %ah,0x72440073(%rip)        # 72442a97 <_end+0x7223dd67>
    2a24:	45 76 69             	rex.RB jbe 2a90 <array.3413+0x210>
    2a27:	6c                   	insb   (%dx),%es:(%rdi)
    2a28:	00 67 72             	add    %ah,0x72(%rdi)
    2a2b:	65 61                	gs (bad) 
    2a2d:	74 77                	je     2aa6 <array.3413+0x226>
    2a2f:	68 69 74 65 2e       	pushq  $0x2e657469
    2a34:	69 63 73 2e 63 73 2e 	imul   $0x2e73632e,0x73(%rbx),%esp
    2a3b:	63 6d 75             	movslq 0x75(%rbp),%ebp
    2a3e:	2e 65 64 75 00       	cs gs fs jne,pn 2a43 <array.3413+0x1c3>
    2a43:	61                   	(bad)  
    2a44:	6e                   	outsb  %ds:(%rsi),(%dx)
    2a45:	67 65 6c             	gs insb (%dx),%es:(%edi)
    2a48:	73 68                	jae    2ab2 <array.3413+0x232>
    2a4a:	61                   	(bad)  
    2a4b:	72 6b                	jb     2ab8 <array.3413+0x238>
    2a4d:	2e 69 63 73 2e 63 73 	imul   $0x2e73632e,%cs:0x73(%rbx),%esp
    2a54:	2e 
    2a55:	63 6d 75             	movslq 0x75(%rbp),%ebp
    2a58:	2e 65 64 75 00       	cs gs fs jne,pn 2a5d <array.3413+0x1dd>
    2a5d:	6d                   	insl   (%dx),%es:(%rdi)
    2a5e:	61                   	(bad)  
    2a5f:	6b 6f 73 68          	imul   $0x68,0x73(%rdi),%ebp
    2a63:	61                   	(bad)  
    2a64:	72 6b                	jb     2ad1 <array.3413+0x251>
    2a66:	2e 69 63 73 2e 63 73 	imul   $0x2e73632e,%cs:0x73(%rbx),%esp
    2a6d:	2e 
    2a6e:	63 6d 75             	movslq 0x75(%rbp),%ebp
    2a71:	2e 65 64 75 00       	cs gs fs jne,pn 2a76 <array.3413+0x1f6>
    2a76:	77 68                	ja     2ae0 <array.3413+0x260>
    2a78:	61                   	(bad)  
    2a79:	6c                   	insb   (%dx),%es:(%rdi)
    2a7a:	65 73 68             	gs jae 2ae5 <array.3413+0x265>
    2a7d:	61                   	(bad)  
    2a7e:	72 6b                	jb     2aeb <array.3413+0x26b>
    2a80:	2e 69 63 73 2e 63 73 	imul   $0x2e73632e,%cs:0x73(%rbx),%esp
    2a87:	2e 
    2a88:	63 6d 75             	movslq 0x75(%rbp),%ebp
    2a8b:	2e 65 64 75 00       	cs gs fs jne,pn 2a90 <array.3413+0x210>
    2a90:	50                   	push   %rax
    2a91:	72 6f                	jb     2b02 <array.3413+0x282>
    2a93:	67 72 61             	addr32 jb 2af7 <array.3413+0x277>
    2a96:	6d                   	insl   (%dx),%es:(%rdi)
    2a97:	20 74 69 6d          	and    %dh,0x6d(%rcx,%rbp,2)
    2a9b:	65 64 20 6f 75       	gs and %ch,%fs:0x75(%rdi)
    2aa0:	74 20                	je     2ac2 <array.3413+0x242>
    2aa2:	61                   	(bad)  
    2aa3:	66 74 65             	data16 je 2b0b <array.3413+0x28b>
    2aa6:	72 20                	jb     2ac8 <array.3413+0x248>
    2aa8:	25 64 20 73 65       	and    $0x65732064,%eax
    2aad:	63 6f 6e             	movslq 0x6e(%rdi),%ebp
    2ab0:	64 73 0a             	fs jae 2abd <array.3413+0x23d>
    2ab3:	00 00                	add    %al,(%rax)
    2ab5:	00 00                	add    %al,(%rax)
    2ab7:	00 45 72             	add    %al,0x72(%rbp)
    2aba:	72 6f                	jb     2b2b <array.3413+0x2ab>
    2abc:	72 3a                	jb     2af8 <array.3413+0x278>
    2abe:	20 48 54             	and    %cl,0x54(%rax)
    2ac1:	54                   	push   %rsp
    2ac2:	50                   	push   %rax
    2ac3:	20 72 65             	and    %dh,0x65(%rdx)
    2ac6:	71 75                	jno    2b3d <array.3413+0x2bd>
    2ac8:	65 73 74             	gs jae 2b3f <array.3413+0x2bf>
    2acb:	20 66 61             	and    %ah,0x61(%rsi)
    2ace:	69 6c 65 64 20 77 69 	imul   $0x74697720,0x64(%rbp,%riz,2),%ebp
    2ad5:	74 
    2ad6:	68 20 65 72 72       	pushq  $0x72726520
    2adb:	6f                   	outsl  %ds:(%rsi),(%dx)
    2adc:	72 20                	jb     2afe <array.3413+0x27e>
    2ade:	25 64 3a 20 25       	and    $0x25203a64,%eax
    2ae3:	73 00                	jae    2ae5 <array.3413+0x265>
    2ae5:	00 00                	add    %al,(%rax)
    2ae7:	00 47 45             	add    %al,0x45(%rdi)
    2aea:	54                   	push   %rsp
    2aeb:	20 2f                	and    %ch,(%rdi)
    2aed:	25 73 2f 73 75       	and    $0x75732f73,%eax
    2af2:	62                   	(bad)  
    2af3:	6d                   	insl   (%dx),%es:(%rdi)
    2af4:	69 74 72 2e 70 6c 2f 	imul   $0x3f2f6c70,0x2e(%rdx,%rsi,2),%esi
    2afb:	3f 
    2afc:	75 73                	jne    2b71 <array.3413+0x2f1>
    2afe:	65 72 69             	gs jb  2b6a <array.3413+0x2ea>
    2b01:	64 3d 25 73 26 75    	fs cmp $0x75267325,%eax
    2b07:	73 65                	jae    2b6e <array.3413+0x2ee>
    2b09:	72 70                	jb     2b7b <array.3413+0x2fb>
    2b0b:	77 64                	ja     2b71 <array.3413+0x2f1>
    2b0d:	3d 25 73 26 6c       	cmp    $0x6c267325,%eax
    2b12:	61                   	(bad)  
    2b13:	62                   	(bad)  
    2b14:	3d 25 73 26 72       	cmp    $0x72267325,%eax
    2b19:	65 73 75             	gs jae 2b91 <array.3413+0x311>
    2b1c:	6c                   	insb   (%dx),%es:(%rdi)
    2b1d:	74 3d                	je     2b5c <array.3413+0x2dc>
    2b1f:	25 73 26 73 75       	and    $0x75732673,%eax
    2b24:	62                   	(bad)  
    2b25:	6d                   	insl   (%dx),%es:(%rdi)
    2b26:	69 74 3d 73 75 62 6d 	imul   $0x696d6275,0x73(%rbp,%rdi,1),%esi
    2b2d:	69 
    2b2e:	74 20                	je     2b50 <array.3413+0x2d0>
    2b30:	48 54                	rex.W push %rsp
    2b32:	54                   	push   %rsp
    2b33:	50                   	push   %rax
    2b34:	2f                   	(bad)  
    2b35:	31 2e                	xor    %ebp,(%rsi)
    2b37:	30 0d 0a 0d 0a 00    	xor    %cl,0xa0d0a(%rip)        # a3847 <__FRAME_END__+0xa069b>
    2b3d:	00 00                	add    %al,(%rax)
    2b3f:	00 45 72             	add    %al,0x72(%rbp)
    2b42:	72 6f                	jb     2bb3 <array.3413+0x333>
    2b44:	72 3a                	jb     2b80 <array.3413+0x300>
    2b46:	20 55 6e             	and    %dl,0x6e(%rbp)
    2b49:	61                   	(bad)  
    2b4a:	62                   	(bad)  
    2b4b:	6c                   	insb   (%dx),%es:(%rdi)
    2b4c:	65 20 74 6f 20       	and    %dh,%gs:0x20(%rdi,%rbp,2)
    2b51:	63 6f 6e             	movslq 0x6e(%rdi),%ebp
    2b54:	6e                   	outsb  %ds:(%rsi),(%dx)
    2b55:	65 63 74 20 74       	movslq %gs:0x74(%rax,%riz,1),%esi
    2b5a:	6f                   	outsl  %ds:(%rsi),(%dx)
    2b5b:	20 73 65             	and    %dh,0x65(%rbx)
    2b5e:	72 76                	jb     2bd6 <__GNU_EH_FRAME_HDR+0x1a>
    2b60:	65 72 20             	gs jb  2b83 <array.3413+0x303>
    2b63:	25 73 00 00 00       	and    $0x73,%eax
    2b68:	25 25 25 30 32       	and    $0x32302525,%eax
    2b6d:	58                   	pop    %rax
    2b6e:	00 25 73 20 25 64    	add    %ah,0x64252073(%rip)        # 64254be7 <_end+0x6404feb7>
    2b74:	20 25 5b 61 2d 7a    	and    %ah,0x7a2d615b(%rip)        # 7a2d8cd5 <_end+0x7a0d3fa5>
    2b7a:	41 2d 7a 20 5d 00    	rex.B sub $0x5d207a,%eax
    2b80:	0d 0a 00 4f 4b       	or     $0x4b4f000a,%eax
    2b85:	00 63 68             	add    %ah,0x68(%rbx)
    2b88:	61                   	(bad)  
    2b89:	6e                   	outsb  %ds:(%rsi),(%dx)
    2b8a:	67 65 6d             	gs insl (%dx),%es:(%edi)
    2b8d:	65 2e 69 63 73 2e 63 	gs imul $0x2e73632e,%cs:0x73(%rbx),%esp
    2b94:	73 2e 
    2b96:	63 6d 75             	movslq 0x75(%rbp),%ebp
    2b99:	2e 65 64 75 00       	cs gs fs jne,pn 2b9e <array.3413+0x31e>
    2b9e:	0a 41 55             	or     0x55(%rcx),%al
    2ba1:	54                   	push   %rsp
    2ba2:	4f 52                	rex.WRXB push %r10
    2ba4:	45 53                	rex.RB push %r11
    2ba6:	55                   	push   %rbp
    2ba7:	4c 54                	rex.WR push %rsp
    2ba9:	5f                   	pop    %rdi
    2baa:	53                   	push   %rbx
    2bab:	54                   	push   %rsp
    2bac:	52                   	push   %rdx
    2bad:	49                   	rex.WB
    2bae:	4e                   	rex.WRX
    2baf:	47 3d 25 73 0a 00    	rex.RXB cmp $0xa7325,%eax
    2bb5:	63 73 61             	movslq 0x61(%rbx),%esi
    2bb8:	70 70                	jo     2c2a <__GNU_EH_FRAME_HDR+0x6e>
	...

Disassembly of section .eh_frame_hdr:

0000000000002bbc <__GNU_EH_FRAME_HDR>:
    2bbc:	01 1b                	add    %ebx,(%rbx)
    2bbe:	03 3b                	add    (%rbx),%edi
    2bc0:	10 01                	adc    %al,(%rcx)
    2bc2:	00 00                	add    %al,(%rax)
    2bc4:	21 00                	and    %eax,(%rax)
    2bc6:	00 00                	add    %al,(%rax)
    2bc8:	34 e2                	xor    $0xe2,%al
    2bca:	ff                   	(bad)  
    2bcb:	ff 5c 01 00          	lcall  *0x0(%rcx,%rax,1)
    2bcf:	00 d4                	add    %dl,%ah
    2bd1:	e3 ff                	jrcxz  2bd2 <__GNU_EH_FRAME_HDR+0x16>
    2bd3:	ff 84 01 00 00 e4 e3 	incl   -0x1c1c0000(%rcx,%rax,1)
    2bda:	ff                   	(bad)  
    2bdb:	ff 2c 01             	ljmp   *(%rcx,%rax,1)
    2bde:	00 00                	add    %al,(%rax)
    2be0:	ee                   	out    %al,(%dx)
    2be1:	e4 ff                	in     $0xff,%al
    2be3:	ff 9c 01 00 00 48 e6 	lcall  *-0x19b80000(%rcx,%rax,1)
    2bea:	ff                   	(bad)  
    2beb:	ff                   	(bad)  
    2bec:	bc 01 00 00 68       	mov    $0x68000001,%esp
    2bf1:	e6 ff                	out    %al,$0xff
    2bf3:	ff                   	(bad)  
    2bf4:	d8 01                	fadds  (%rcx)
    2bf6:	00 00                	add    %al,(%rax)
    2bf8:	d7                   	xlat   %ds:(%rbx)
    2bf9:	e6 ff                	out    %al,$0xff
    2bfb:	ff 04 02             	incl   (%rdx,%rax,1)
    2bfe:	00 00                	add    %al,(%rax)
    2c00:	8b e7                	mov    %edi,%esp
    2c02:	ff                   	(bad)  
    2c03:	ff 20                	jmpq   *(%rax)
    2c05:	02 00                	add    (%rax),%al
    2c07:	00 c4                	add    %al,%ah
    2c09:	e7 ff                	out    %eax,$0xff
    2c0b:	ff 4c 02 00          	decl   0x0(%rdx,%rax,1)
    2c0f:	00 33                	add    %dh,(%rbx)
    2c11:	e8 ff ff 68 02       	callq  2692c15 <_end+0x248dee5>
    2c16:	00 00                	add    %al,(%rax)
    2c18:	c6                   	(bad)  
    2c19:	e8 ff ff 84 02       	callq  2852c1d <_end+0x264deed>
    2c1e:	00 00                	add    %al,(%rax)
    2c20:	e8 e9 ff ff c4       	callq  ffffffffc5002c0e <_end+0xffffffffc4dfdede>
    2c25:	02 00                	add    (%rax),%al
    2c27:	00 27                	add    %ah,(%rdi)
    2c29:	ea                   	(bad)  
    2c2a:	ff                   	(bad)  
    2c2b:	ff e4                	jmpq   *%rsp
    2c2d:	02 00                	add    (%rax),%al
    2c2f:	00 7f ea             	add    %bh,-0x16(%rdi)
    2c32:	ff                   	(bad)  
    2c33:	ff 04 03             	incl   (%rbx,%rax,1)
    2c36:	00 00                	add    %al,(%rax)
    2c38:	db ea                	fucomi %st(2),%st
    2c3a:	ff                   	(bad)  
    2c3b:	ff 18                	lcall  *(%rax)
    2c3d:	03 00                	add    (%rax),%eax
    2c3f:	00 02                	add    %al,(%rdx)
    2c41:	eb ff                	jmp    2c42 <__GNU_EH_FRAME_HDR+0x86>
    2c43:	ff 2c 03             	ljmp   *(%rbx,%rax,1)
    2c46:	00 00                	add    %al,(%rax)
    2c48:	1f                   	(bad)  
    2c49:	eb ff                	jmp    2c4a <__GNU_EH_FRAME_HDR+0x8e>
    2c4b:	ff 40 03             	incl   0x3(%rax)
    2c4e:	00 00                	add    %al,(%rax)
    2c50:	86 eb                	xchg   %ch,%bl
    2c52:	ff                   	(bad)  
    2c53:	ff 6c 03 00          	ljmp   *0x0(%rbx,%rax,1)
    2c57:	00 a0 eb ff ff 84    	add    %ah,-0x7b000015(%rax)
    2c5d:	03 00                	add    (%rax),%eax
    2c5f:	00 a2 eb ff ff 98    	add    %ah,-0x67000015(%rdx)
    2c65:	03 00                	add    (%rax),%eax
    2c67:	00 dd                	add    %bl,%ch
    2c69:	eb ff                	jmp    2c6a <__GNU_EH_FRAME_HDR+0xae>
    2c6b:	ff c0                	inc    %eax
    2c6d:	03 00                	add    (%rax),%eax
    2c6f:	00 2b                	add    %ch,(%rbx)
    2c71:	ec                   	in     (%dx),%al
    2c72:	ff                   	(bad)  
    2c73:	ff                   	(bad)  
    2c74:	e8 03 00 00 51       	callq  51002c7c <_end+0x50dfdf4c>
    2c79:	ec                   	in     (%dx),%al
    2c7a:	ff                   	(bad)  
    2c7b:	ff                   	(bad)  
    2c7c:	fc                   	cld    
    2c7d:	03 00                	add    (%rax),%eax
    2c7f:	00 92 ec ff ff 20    	add    %dl,0x20ffffec(%rdx)
    2c85:	04 00                	add    $0x0,%al
    2c87:	00 d6                	add    %dl,%dh
    2c89:	ed                   	in     (%dx),%eax
    2c8a:	ff                   	(bad)  
    2c8b:	ff                   	(bad)  
    2c8c:	3c 04                	cmp    $0x4,%al
    2c8e:	00 00                	add    %al,(%rax)
    2c90:	7b ee                	jnp    2c80 <__GNU_EH_FRAME_HDR+0xc4>
    2c92:	ff                   	(bad)  
    2c93:	ff 5c 04 00          	lcall  *0x0(%rsp,%rax,1)
    2c97:	00 ab ee ff ff 70    	add    %ch,0x70ffffee(%rbx)
    2c9d:	04 00                	add    $0x0,%al
    2c9f:	00 67 ef             	add    %ah,-0x11(%rdi)
    2ca2:	ff                   	(bad)  
    2ca3:	ff b0 04 00 00 a8    	pushq  -0x57fffffc(%rax)
    2ca9:	f7 ff                	idiv   %edi
    2cab:	ff 1c 05 00 00 d3 f7 	lcall  *-0x82d0000(,%rax,1)
    2cb2:	ff                   	(bad)  
    2cb3:	ff                   	(bad)  
    2cb4:	38 05 00 00 dc f9    	cmp    %al,-0x6240000(%rip)        # fffffffff9dc2cba <_end+0xfffffffff9bbdf8a>
    2cba:	ff                   	(bad)  
    2cbb:	ff 6c 05 00          	ljmp   *0x0(%rbp,%rax,1)
    2cbf:	00 54 fa ff          	add    %dl,-0x1(%rdx,%rdi,8)
    2cc3:	ff 94 05 00 00 c4 fa 	callq  *-0x53c0000(%rbp,%rax,1)
    2cca:	ff                   	(bad)  
    2ccb:	ff                   	(bad)  
    2ccc:	dc                   	.byte 0xdc
    2ccd:	05                   	.byte 0x5
	...

Disassembly of section .eh_frame:

0000000000002cd0 <__FRAME_END__-0x4dc>:
    2cd0:	14 00                	adc    $0x0,%al
    2cd2:	00 00                	add    %al,(%rax)
    2cd4:	00 00                	add    %al,(%rax)
    2cd6:	00 00                	add    %al,(%rax)
    2cd8:	01 7a 52             	add    %edi,0x52(%rdx)
    2cdb:	00 01                	add    %al,(%rcx)
    2cdd:	78 10                	js     2cef <__GNU_EH_FRAME_HDR+0x133>
    2cdf:	01 1b                	add    %ebx,(%rbx)
    2ce1:	0c 07                	or     $0x7,%al
    2ce3:	08 90 01 07 10 14    	or     %dl,0x14100701(%rax)
    2ce9:	00 00                	add    %al,(%rax)
    2ceb:	00 1c 00             	add    %bl,(%rax,%rax,1)
    2cee:	00 00                	add    %al,(%rax)
    2cf0:	b0 e2                	mov    $0xe2,%al
    2cf2:	ff                   	(bad)  
    2cf3:	ff 2b                	ljmp   *(%rbx)
	...
    2cfd:	00 00                	add    %al,(%rax)
    2cff:	00 14 00             	add    %dl,(%rax,%rax,1)
    2d02:	00 00                	add    %al,(%rax)
    2d04:	00 00                	add    %al,(%rax)
    2d06:	00 00                	add    %al,(%rax)
    2d08:	01 7a 52             	add    %edi,0x52(%rdx)
    2d0b:	00 01                	add    %al,(%rcx)
    2d0d:	78 10                	js     2d1f <__GNU_EH_FRAME_HDR+0x163>
    2d0f:	01 1b                	add    %ebx,(%rbx)
    2d11:	0c 07                	or     $0x7,%al
    2d13:	08 90 01 00 00 24    	or     %dl,0x24000001(%rax)
    2d19:	00 00                	add    %al,(%rax)
    2d1b:	00 1c 00             	add    %bl,(%rax,%rax,1)
    2d1e:	00 00                	add    %al,(%rax)
    2d20:	d0 e0                	shl    %al
    2d22:	ff                   	(bad)  
    2d23:	ff a0 01 00 00 00    	jmpq   *0x1(%rax)
    2d29:	0e                   	(bad)  
    2d2a:	10 46 0e             	adc    %al,0xe(%rsi)
    2d2d:	18 4a 0f             	sbb    %cl,0xf(%rdx)
    2d30:	0b 77 08             	or     0x8(%rdi),%esi
    2d33:	80 00 3f             	addb   $0x3f,(%rax)
    2d36:	1a 3b                	sbb    (%rbx),%bh
    2d38:	2a 33                	sub    (%rbx),%dh
    2d3a:	24 22                	and    $0x22,%al
    2d3c:	00 00                	add    %al,(%rax)
    2d3e:	00 00                	add    %al,(%rax)
    2d40:	14 00                	adc    $0x0,%al
    2d42:	00 00                	add    %al,(%rax)
    2d44:	44 00 00             	add    %r8b,(%rax)
    2d47:	00 48 e2             	add    %cl,-0x1e(%rax)
    2d4a:	ff                   	(bad)  
    2d4b:	ff 08                	decl   (%rax)
	...
    2d55:	00 00                	add    %al,(%rax)
    2d57:	00 1c 00             	add    %bl,(%rax,%rax,1)
    2d5a:	00 00                	add    %al,(%rax)
    2d5c:	5c                   	pop    %rsp
    2d5d:	00 00                	add    %al,(%rax)
    2d5f:	00 4a e3             	add    %cl,-0x1d(%rdx)
    2d62:	ff                   	(bad)  
    2d63:	ff 5a 01             	lcall  *0x1(%rdx)
    2d66:	00 00                	add    %al,(%rax)
    2d68:	00 41 0e             	add    %al,0xe(%rcx)
    2d6b:	10 83 02 03 00 01    	adc    %al,0x1000302(%rbx)
    2d71:	0a 0e                	or     (%rsi),%cl
    2d73:	08 41 0b             	or     %al,0xb(%rcx)
    2d76:	00 00                	add    %al,(%rax)
    2d78:	18 00                	sbb    %al,(%rax)
    2d7a:	00 00                	add    %al,(%rax)
    2d7c:	7c 00                	jl     2d7e <__GNU_EH_FRAME_HDR+0x1c2>
    2d7e:	00 00                	add    %al,(%rax)
    2d80:	84 e4                	test   %ah,%ah
    2d82:	ff                   	(bad)  
    2d83:	ff 20                	jmpq   *(%rax)
    2d85:	00 00                	add    %al,(%rax)
    2d87:	00 00                	add    %al,(%rax)
    2d89:	44 0e                	rex.R (bad) 
    2d8b:	10 54 0a 0e          	adc    %dl,0xe(%rdx,%rcx,1)
    2d8f:	08 41 0b             	or     %al,0xb(%rcx)
    2d92:	00 00                	add    %al,(%rax)
    2d94:	28 00                	sub    %al,(%rax)
    2d96:	00 00                	add    %al,(%rax)
    2d98:	98                   	cwtl   
    2d99:	00 00                	add    %al,(%rax)
    2d9b:	00 88 e4 ff ff 6f    	add    %cl,0x6fffffe4(%rax)
    2da1:	00 00                	add    %al,(%rax)
    2da3:	00 00                	add    %al,(%rax)
    2da5:	41 0e                	rex.B (bad) 
    2da7:	10 86 02 41 0e 18    	adc    %al,0x180e4102(%rsi)
    2dad:	83 03 44             	addl   $0x44,(%rbx)
    2db0:	0e                   	(bad)  
    2db1:	40 02 61 0a          	add    0xa(%rcx),%spl
    2db5:	0e                   	(bad)  
    2db6:	18 41 0e             	sbb    %al,0xe(%rcx)
    2db9:	10 41 0e             	adc    %al,0xe(%rcx)
    2dbc:	08 41 0b             	or     %al,0xb(%rcx)
    2dbf:	00 18                	add    %bl,(%rax)
    2dc1:	00 00                	add    %al,(%rax)
    2dc3:	00 c4                	add    %al,%ah
    2dc5:	00 00                	add    %al,(%rax)
    2dc7:	00 cb                	add    %cl,%bl
    2dc9:	e4 ff                	in     $0xff,%al
    2dcb:	ff b4 00 00 00 00 44 	pushq  0x44000000(%rax,%rax,1)
    2dd2:	0e                   	(bad)  
    2dd3:	20 02                	and    %al,(%rdx)
    2dd5:	aa                   	stos   %al,%es:(%rdi)
    2dd6:	0a 0e                	or     (%rsi),%cl
    2dd8:	08 41 0b             	or     %al,0xb(%rcx)
    2ddb:	00 28                	add    %ch,(%rax)
    2ddd:	00 00                	add    %al,(%rax)
    2ddf:	00 e0                	add    %ah,%al
    2de1:	00 00                	add    %al,(%rax)
    2de3:	00 63 e5             	add    %ah,-0x1b(%rbx)
    2de6:	ff                   	(bad)  
    2de7:	ff                   	(bad)  
    2de8:	39 00                	cmp    %eax,(%rax)
    2dea:	00 00                	add    %al,(%rax)
    2dec:	00 54 0e 10          	add    %dl,0x10(%rsi,%rcx,1)
    2df0:	8c 02                	mov    %es,(%rdx)
    2df2:	41 0e                	rex.B (bad) 
    2df4:	18 86 03 41 0e 20    	sbb    %al,0x200e4103(%rsi)
    2dfa:	83 04 5f 0e          	addl   $0xe,(%rdi,%rbx,2)
    2dfe:	18 41 0e             	sbb    %al,0xe(%rcx)
    2e01:	10 42 0e             	adc    %al,0xe(%rdx)
    2e04:	08 00                	or     %al,(%rax)
    2e06:	00 00                	add    %al,(%rax)
    2e08:	18 00                	sbb    %al,(%rax)
    2e0a:	00 00                	add    %al,(%rax)
    2e0c:	0c 01                	or     $0x1,%al
    2e0e:	00 00                	add    %al,(%rax)
    2e10:	70 e5                	jo     2df7 <__GNU_EH_FRAME_HDR+0x23b>
    2e12:	ff                   	(bad)  
    2e13:	ff 6f 00             	ljmp   *0x0(%rdi)
    2e16:	00 00                	add    %al,(%rax)
    2e18:	00 44 0e 20          	add    %al,0x20(%rsi,%rcx,1)
    2e1c:	02 65 0a             	add    0xa(%rbp),%ah
    2e1f:	0e                   	(bad)  
    2e20:	08 41 0b             	or     %al,0xb(%rcx)
    2e23:	00 18                	add    %bl,(%rax)
    2e25:	00 00                	add    %al,(%rax)
    2e27:	00 28                	add    %ch,(%rax)
    2e29:	01 00                	add    %eax,(%rax)
    2e2b:	00 c3                	add    %al,%bl
    2e2d:	e5 ff                	in     $0xff,%eax
    2e2f:	ff 93 00 00 00 00    	callq  *0x0(%rbx)
    2e35:	44 0e                	rex.R (bad) 
    2e37:	20 02                	and    %al,(%rdx)
    2e39:	82                   	(bad)  
    2e3a:	0a 0e                	or     (%rsi),%cl
    2e3c:	08 41 0b             	or     %al,0xb(%rcx)
    2e3f:	00 3c 00             	add    %bh,(%rax,%rax,1)
    2e42:	00 00                	add    %al,(%rax)
    2e44:	44 01 00             	add    %r8d,(%rax)
    2e47:	00 3a                	add    %bh,(%rdx)
    2e49:	e6 ff                	out    %al,$0xff
    2e4b:	ff 22                	jmpq   *(%rdx)
    2e4d:	01 00                	add    %eax,(%rax)
    2e4f:	00 00                	add    %al,(%rax)
    2e51:	42 0e                	rex.X (bad) 
    2e53:	10 8d 02 42 0e 18    	adc    %cl,0x180e4202(%rbp)
    2e59:	8c 03                	mov    %es,(%rbx)
    2e5b:	41 0e                	rex.B (bad) 
    2e5d:	20 86 04 41 0e 28    	and    %al,0x280e4104(%rsi)
    2e63:	83 05 44 0e 90 01 03 	addl   $0x3,0x1900e44(%rip)        # 1903cae <_end+0x16fef7e>
    2e6a:	0c 01                	or     $0x1,%al
    2e6c:	0a 0e                	or     (%rsi),%cl
    2e6e:	28 41 0e             	sub    %al,0xe(%rcx)
    2e71:	20 41 0e             	and    %al,0xe(%rcx)
    2e74:	18 42 0e             	sbb    %al,0xe(%rdx)
    2e77:	10 42 0e             	adc    %al,0xe(%rdx)
    2e7a:	08 41 0b             	or     %al,0xb(%rcx)
    2e7d:	00 00                	add    %al,(%rax)
    2e7f:	00 1c 00             	add    %bl,(%rax,%rax,1)
    2e82:	00 00                	add    %al,(%rax)
    2e84:	84 01                	test   %al,(%rcx)
    2e86:	00 00                	add    %al,(%rax)
    2e88:	1c e7                	sbb    $0xe7,%al
    2e8a:	ff                   	(bad)  
    2e8b:	ff                   	(bad)  
    2e8c:	3f                   	(bad)  
    2e8d:	00 00                	add    %al,(%rax)
    2e8f:	00 00                	add    %al,(%rax)
    2e91:	49 0e                	rex.WB (bad) 
    2e93:	10 53 0a             	adc    %dl,0xa(%rbx)
    2e96:	0e                   	(bad)  
    2e97:	08 41 0b             	or     %al,0xb(%rcx)
    2e9a:	5c                   	pop    %rsp
    2e9b:	0e                   	(bad)  
    2e9c:	08 00                	or     %al,(%rax)
    2e9e:	00 00                	add    %al,(%rax)
    2ea0:	1c 00                	sbb    $0x0,%al
    2ea2:	00 00                	add    %al,(%rax)
    2ea4:	a4                   	movsb  %ds:(%rsi),%es:(%rdi)
    2ea5:	01 00                	add    %eax,(%rax)
    2ea7:	00 3b                	add    %bh,(%rbx)
    2ea9:	e7 ff                	out    %eax,$0xff
    2eab:	ff 58 00             	lcall  *0x0(%rax)
    2eae:	00 00                	add    %al,(%rax)
    2eb0:	00 41 0e             	add    %al,0xe(%rcx)
    2eb3:	10 83 02 02 48 0a    	adc    %al,0xa480202(%rbx)
    2eb9:	0e                   	(bad)  
    2eba:	08 41 0b             	or     %al,0xb(%rcx)
    2ebd:	00 00                	add    %al,(%rax)
    2ebf:	00 10                	add    %dl,(%rax)
    2ec1:	00 00                	add    %al,(%rax)
    2ec3:	00 c4                	add    %al,%ah
    2ec5:	01 00                	add    %eax,(%rax)
    2ec7:	00 73 e7             	add    %dh,-0x19(%rbx)
    2eca:	ff                   	(bad)  
    2ecb:	ff 5c 00 00          	lcall  *0x0(%rax,%rax,1)
    2ecf:	00 00                	add    %al,(%rax)
    2ed1:	44 0e                	rex.R (bad) 
    2ed3:	10 10                	adc    %dl,(%rax)
    2ed5:	00 00                	add    %al,(%rax)
    2ed7:	00 d8                	add    %bl,%al
    2ed9:	01 00                	add    %eax,(%rax)
    2edb:	00 bb e7 ff ff 27    	add    %bh,0x27ffffe7(%rbx)
    2ee1:	00 00                	add    %al,(%rax)
    2ee3:	00 00                	add    %al,(%rax)
    2ee5:	44 0e                	rex.R (bad) 
    2ee7:	10 10                	adc    %dl,(%rax)
    2ee9:	00 00                	add    %al,(%rax)
    2eeb:	00 ec                	add    %ch,%ah
    2eed:	01 00                	add    %eax,(%rax)
    2eef:	00 ce                	add    %cl,%dh
    2ef1:	e7 ff                	out    %eax,$0xff
    2ef3:	ff 1d 00 00 00 00    	lcall  *0x0(%rip)        # 2ef9 <__GNU_EH_FRAME_HDR+0x33d>
    2ef9:	00 00                	add    %al,(%rax)
    2efb:	00 28                	add    %ch,(%rax)
    2efd:	00 00                	add    %al,(%rax)
    2eff:	00 00                	add    %al,(%rax)
    2f01:	02 00                	add    (%rax),%al
    2f03:	00 d7                	add    %dl,%bh
    2f05:	e7 ff                	out    %eax,$0xff
    2f07:	ff 67 00             	jmpq   *0x0(%rdi)
    2f0a:	00 00                	add    %al,(%rax)
    2f0c:	00 42 0e             	add    %al,0xe(%rdx)
    2f0f:	10 8c 02 41 0e 18 86 	adc    %cl,-0x79e7f1bf(%rdx,%rax,1)
    2f16:	03 41 0e             	add    0xe(%rcx),%eax
    2f19:	20 83 04 63 0a 0e    	and    %al,0xe0a6304(%rbx)
    2f1f:	18 41 0e             	sbb    %al,0xe(%rcx)
    2f22:	10 42 0e             	adc    %al,0xe(%rdx)
    2f25:	08 41 0b             	or     %al,0xb(%rcx)
    2f28:	14 00                	adc    $0x0,%al
    2f2a:	00 00                	add    %al,(%rax)
    2f2c:	2c 02                	sub    $0x2,%al
    2f2e:	00 00                	add    %al,(%rax)
    2f30:	12 e8                	adc    %al,%ch
    2f32:	ff                   	(bad)  
    2f33:	ff 1a                	lcall  *(%rdx)
    2f35:	00 00                	add    %al,(%rax)
    2f37:	00 00                	add    %al,(%rax)
    2f39:	44 0e                	rex.R (bad) 
    2f3b:	10 55 0e             	adc    %dl,0xe(%rbp)
    2f3e:	08 00                	or     %al,(%rax)
    2f40:	10 00                	adc    %al,(%rax)
    2f42:	00 00                	add    %al,(%rax)
    2f44:	44 02 00             	add    (%rax),%r8b
    2f47:	00 14 e8             	add    %dl,(%rax,%rbp,8)
    2f4a:	ff                   	(bad)  
    2f4b:	ff 02                	incl   (%rdx)
    2f4d:	00 00                	add    %al,(%rax)
    2f4f:	00 00                	add    %al,(%rax)
    2f51:	00 00                	add    %al,(%rax)
    2f53:	00 24 00             	add    %ah,(%rax,%rax,1)
    2f56:	00 00                	add    %al,(%rax)
    2f58:	58                   	pop    %rax
    2f59:	02 00                	add    (%rax),%al
    2f5b:	00 02                	add    %al,(%rdx)
    2f5d:	e8 ff ff 3b 00       	callq  3c2f61 <_end+0x1be231>
    2f62:	00 00                	add    %al,(%rax)
    2f64:	00 41 0e             	add    %al,0xe(%rcx)
    2f67:	10 86 02 41 0e 18    	adc    %al,0x180e4102(%rsi)
    2f6d:	83 03 44             	addl   $0x44,(%rbx)
    2f70:	0e                   	(bad)  
    2f71:	20 72 0e             	and    %dh,0xe(%rdx)
    2f74:	18 41 0e             	sbb    %al,0xe(%rcx)
    2f77:	10 41 0e             	adc    %al,0xe(%rcx)
    2f7a:	08 00                	or     %al,(%rax)
    2f7c:	24 00                	and    $0x0,%al
    2f7e:	00 00                	add    %al,(%rax)
    2f80:	80 02 00             	addb   $0x0,(%rdx)
    2f83:	00 15 e8 ff ff 4e    	add    %dl,0x4effffe8(%rip)        # 4f002f71 <_end+0x4edfe241>
    2f89:	00 00                	add    %al,(%rax)
    2f8b:	00 00                	add    %al,(%rax)
    2f8d:	41 0e                	rex.B (bad) 
    2f8f:	10 86 02 41 0e 18    	adc    %al,0x180e4102(%rsi)
    2f95:	83 03 44             	addl   $0x44,(%rbx)
    2f98:	0e                   	(bad)  
    2f99:	20 02                	and    %al,(%rdx)
    2f9b:	45 0e                	rex.RB (bad) 
    2f9d:	18 41 0e             	sbb    %al,0xe(%rcx)
    2fa0:	10 41 0e             	adc    %al,0xe(%rcx)
    2fa3:	08 10                	or     %dl,(%rax)
    2fa5:	00 00                	add    %al,(%rax)
    2fa7:	00 a8 02 00 00 3b    	add    %ch,0x3b000002(%rax)
    2fad:	e8 ff ff 26 00       	callq  272fb1 <_end+0x6e281>
    2fb2:	00 00                	add    %al,(%rax)
    2fb4:	00 44 0e 10          	add    %al,0x10(%rsi,%rcx,1)
    2fb8:	20 00                	and    %al,(%rax)
    2fba:	00 00                	add    %al,(%rax)
    2fbc:	bc 02 00 00 4d       	mov    $0x4d000002,%esp
    2fc1:	e8 ff ff 41 00       	callq  422fc5 <_end+0x21e295>
    2fc6:	00 00                	add    %al,(%rax)
    2fc8:	00 44 0e 10          	add    %al,0x10(%rsi,%rcx,1)
    2fcc:	4c 0e                	rex.WR (bad) 
    2fce:	18 45 0e             	sbb    %al,0xe(%rbp)
    2fd1:	20 5d 0e             	and    %bl,0xe(%rbp)
    2fd4:	10 49 0a             	adc    %cl,0xa(%rcx)
    2fd7:	0e                   	(bad)  
    2fd8:	08 41 0b             	or     %al,0xb(%rcx)
    2fdb:	00 18                	add    %bl,(%rax)
    2fdd:	00 00                	add    %al,(%rax)
    2fdf:	00 e0                	add    %ah,%al
    2fe1:	02 00                	add    (%rax),%al
    2fe3:	00 6a e8             	add    %ch,-0x18(%rdx)
    2fe6:	ff                   	(bad)  
    2fe7:	ff 44 01 00          	incl   0x0(%rcx,%rax,1)
    2feb:	00 00                	add    %al,(%rax)
    2fed:	44 0e                	rex.R (bad) 
    2fef:	10 02                	adc    %al,(%rdx)
    2ff1:	7d 0a                	jge    2ffd <__GNU_EH_FRAME_HDR+0x441>
    2ff3:	0e                   	(bad)  
    2ff4:	08 41 0b             	or     %al,0xb(%rcx)
    2ff7:	00 1c 00             	add    %bl,(%rax,%rax,1)
    2ffa:	00 00                	add    %al,(%rax)
    2ffc:	fc                   	cld    
    2ffd:	02 00                	add    (%rax),%al
    2fff:	00 92 e9 ff ff a5    	add    %dl,-0x5a000017(%rdx)
    3005:	00 00                	add    %al,(%rax)
    3007:	00 00                	add    %al,(%rax)
    3009:	44 0e                	rex.R (bad) 
    300b:	80 01 6d             	addb   $0x6d,(%rcx)
    300e:	0a 0e                	or     (%rsi),%cl
    3010:	08 41 0b             	or     %al,0xb(%rcx)
    3013:	00 00                	add    %al,(%rax)
    3015:	00 00                	add    %al,(%rax)
    3017:	00 10                	add    %dl,(%rax)
    3019:	00 00                	add    %al,(%rax)
    301b:	00 1c 03             	add    %bl,(%rbx,%rax,1)
    301e:	00 00                	add    %al,(%rax)
    3020:	17                   	(bad)  
    3021:	ea                   	(bad)  
    3022:	ff                   	(bad)  
    3023:	ff 30                	pushq  (%rax)
    3025:	00 00                	add    %al,(%rax)
    3027:	00 00                	add    %al,(%rax)
    3029:	44 0e                	rex.R (bad) 
    302b:	10 3c 00             	adc    %bh,(%rax,%rax,1)
    302e:	00 00                	add    %al,(%rax)
    3030:	30 03                	xor    %al,(%rbx)
    3032:	00 00                	add    %al,(%rax)
    3034:	33 ea                	xor    %edx,%ebp
    3036:	ff                   	(bad)  
    3037:	ff                   	(bad)  
    3038:	bc 00 00 00 00       	mov    $0x0,%esp
    303d:	42 0e                	rex.X (bad) 
    303f:	10 8e 02 42 0e 18    	adc    %cl,0x180e4202(%rsi)
    3045:	8d 03                	lea    (%rbx),%eax
    3047:	42 0e                	rex.X (bad) 
    3049:	20 8c 04 41 0e 28 86 	and    %cl,-0x79d7f1bf(%rsp,%rax,1)
    3050:	05 41 0e 30 83       	add    $0x83300e41,%eax
    3055:	06                   	(bad)  
    3056:	02 84 0a 0e 28 41 0e 	add    0xe41280e(%rdx,%rcx,1),%al
    305d:	20 42 0e             	and    %al,0xe(%rdx)
    3060:	18 42 0e             	sbb    %al,0xe(%rdx)
    3063:	10 42 0e             	adc    %al,0xe(%rdx)
    3066:	08 41 0b             	or     %al,0xb(%rcx)
    3069:	00 00                	add    %al,(%rax)
    306b:	00 68 00             	add    %ch,0x0(%rax)
    306e:	00 00                	add    %al,(%rax)
    3070:	70 03                	jo     3075 <__GNU_EH_FRAME_HDR+0x4b9>
    3072:	00 00                	add    %al,(%rax)
    3074:	af                   	scas   %es:(%rdi),%eax
    3075:	ea                   	(bad)  
    3076:	ff                   	(bad)  
    3077:	ff 41 08             	incl   0x8(%rcx)
    307a:	00 00                	add    %al,(%rax)
    307c:	00 42 0e             	add    %al,0xe(%rdx)
    307f:	10 8f 02 42 0e 18    	adc    %cl,0x180e4202(%rdi)
    3085:	8e 03                	mov    (%rbx),%es
    3087:	42 0e                	rex.X (bad) 
    3089:	20 8d 04 42 0e 28    	and    %cl,0x280e4204(%rbp)
    308f:	8c 05 41 0e 30 86    	mov    %es,-0x79cff1bf(%rip)        # ffffffff86303ed6 <_end+0xffffffff860ff1a6>
    3095:	06                   	(bad)  
    3096:	41 0e                	rex.B (bad) 
    3098:	38 83 07 47 0e b0    	cmp    %al,-0x4ff1b8f9(%rbx)
    309e:	c1 02 03             	roll   $0x3,(%rdx)
    30a1:	9d                   	popfq  
    30a2:	06                   	(bad)  
    30a3:	0a 0e                	or     (%rsi),%cl
    30a5:	38 41 0e             	cmp    %al,0xe(%rcx)
    30a8:	30 41 0e             	xor    %al,0xe(%rcx)
    30ab:	28 42 0e             	sub    %al,0xe(%rdx)
    30ae:	20 42 0e             	and    %al,0xe(%rdx)
    30b1:	18 42 0e             	sbb    %al,0xe(%rdx)
    30b4:	10 42 0e             	adc    %al,0xe(%rdx)
    30b7:	08 41 0b             	or     %al,0xb(%rcx)
    30ba:	03 0c 01             	add    (%rcx,%rax,1),%ecx
    30bd:	0e                   	(bad)  
    30be:	b8 c1 02 49 0e       	mov    $0xe4902c1,%eax
    30c3:	c0 c1 02             	rol    $0x2,%cl
    30c6:	44 0e                	rex.R (bad) 
    30c8:	c8 c1 02 44          	enterq $0x2c1,$0x44
    30cc:	0e                   	(bad)  
    30cd:	d0 c1                	rol    %cl
    30cf:	02 02                	add    (%rdx),%al
    30d1:	47 0e                	rex.RXB (bad) 
    30d3:	b0 c1                	mov    $0xc1,%al
    30d5:	02 00                	add    (%rax),%al
    30d7:	00 18                	add    %bl,(%rax)
    30d9:	00 00                	add    %al,(%rax)
    30db:	00 dc                	add    %bl,%ah
    30dd:	03 00                	add    (%rax),%eax
    30df:	00 84 f2 ff ff 2b 00 	add    %al,0x2bffff(%rdx,%rsi,8)
    30e6:	00 00                	add    %al,(%rax)
    30e8:	00 45 0e             	add    %al,0xe(%rbp)
    30eb:	10 83 02 63 0e 08    	adc    %al,0x80e6302(%rbx)
    30f1:	41 c3                	rex.B retq 
    30f3:	00 30                	add    %dh,(%rax)
    30f5:	00 00                	add    %al,(%rax)
    30f7:	00 f8                	add    %bh,%al
    30f9:	03 00                	add    (%rax),%eax
    30fb:	00 93 f2 ff ff 09    	add    %dl,0x9fffff2(%rbx)
    3101:	02 00                	add    (%rax),%al
    3103:	00 00                	add    %al,(%rax)
    3105:	42 0e                	rex.X (bad) 
    3107:	10 8c 02 41 0e 18 86 	adc    %cl,-0x79e7f1bf(%rdx,%rax,1)
    310e:	03 41 0e             	add    0xe(%rcx),%eax
    3111:	20 83 04 44 0e 40    	and    %al,0x400e4404(%rbx)
    3117:	02 fa                	add    %dl,%bh
    3119:	0a 0e                	or     (%rsi),%cl
    311b:	20 41 0e             	and    %al,0xe(%rcx)
    311e:	18 41 0e             	sbb    %al,0xe(%rcx)
    3121:	10 42 0e             	adc    %al,0xe(%rdx)
    3124:	08 41 0b             	or     %al,0xb(%rcx)
    3127:	00 24 00             	add    %ah,(%rax,%rax,1)
    312a:	00 00                	add    %al,(%rax)
    312c:	2c 04                	sub    $0x4,%al
    312e:	00 00                	add    %al,(%rax)
    3130:	68 f4 ff ff 73       	pushq  $0x73fffff4
    3135:	00 00                	add    %al,(%rax)
    3137:	00 00                	add    %al,(%rax)
    3139:	41 0e                	rex.B (bad) 
    313b:	10 83 02 5d 0a 0e    	adc    %al,0xe0a5d02(%rbx)
    3141:	08 41 0b             	or     %al,0xb(%rcx)
    3144:	68 0e 18 41 0e       	pushq  $0xe41180e
    3149:	20 69 0e             	and    %ch,0xe(%rcx)
    314c:	10 00                	adc    %al,(%rax)
    314e:	00 00                	add    %al,(%rax)
    3150:	44 00 00             	add    %r8b,(%rax)
    3153:	00 54 04 00          	add    %dl,0x0(%rsp,%rax,1)
    3157:	00 b8 f4 ff ff 65    	add    %bh,0x65fffff4(%rax)
    315d:	00 00                	add    %al,(%rax)
    315f:	00 00                	add    %al,(%rax)
    3161:	42 0e                	rex.X (bad) 
    3163:	10 8f 02 42 0e 18    	adc    %cl,0x180e4202(%rdi)
    3169:	8e 03                	mov    (%rbx),%es
    316b:	45 0e                	rex.RB (bad) 
    316d:	20 8d 04 42 0e 28    	and    %cl,0x280e4204(%rbp)
    3173:	8c 05 48 0e 30 86    	mov    %es,-0x79cff1b8(%rip)        # ffffffff86303fc1 <_end+0xffffffff860ff291>
    3179:	06                   	(bad)  
    317a:	48 0e                	rex.W (bad) 
    317c:	38 83 07 4d 0e 40    	cmp    %al,0x400e4d07(%rbx)
    3182:	72 0e                	jb     3192 <__GNU_EH_FRAME_HDR+0x5d6>
    3184:	38 41 0e             	cmp    %al,0xe(%rcx)
    3187:	30 41 0e             	xor    %al,0xe(%rcx)
    318a:	28 42 0e             	sub    %al,0xe(%rdx)
    318d:	20 42 0e             	and    %al,0xe(%rdx)
    3190:	18 42 0e             	sbb    %al,0xe(%rdx)
    3193:	10 42 0e             	adc    %al,0xe(%rdx)
    3196:	08 00                	or     %al,(%rax)
    3198:	10 00                	adc    %al,(%rax)
    319a:	00 00                	add    %al,(%rax)
    319c:	9c                   	pushfq 
    319d:	04 00                	add    $0x0,%al
    319f:	00 e0                	add    %ah,%al
    31a1:	f4                   	hlt    
    31a2:	ff                   	(bad)  
    31a3:	ff 02                	incl   (%rdx)
    31a5:	00 00                	add    %al,(%rax)
    31a7:	00 00                	add    %al,(%rax)
    31a9:	00 00                	add    %al,(%rax)
	...

00000000000031ac <__FRAME_END__>:
    31ac:	00 00                	add    %al,(%rax)
	...

Disassembly of section .init_array:

0000000000203cf8 <__frame_dummy_init_array_entry>:
  203cf8:	a0                   	.byte 0xa0
  203cf9:	10 00                	adc    %al,(%rax)
  203cfb:	00 00                	add    %al,(%rax)
  203cfd:	00 00                	add    %al,(%rax)
	...

Disassembly of section .fini_array:

0000000000203d00 <__do_global_dtors_aux_fini_array_entry>:
  203d00:	60                   	(bad)  
  203d01:	10 00                	adc    %al,(%rax)
  203d03:	00 00                	add    %al,(%rax)
  203d05:	00 00                	add    %al,(%rax)
	...

Disassembly of section .dynamic:

0000000000203d08 <_DYNAMIC>:
  203d08:	01 00                	add    %eax,(%rax)
  203d0a:	00 00                	add    %al,(%rax)
  203d0c:	00 00                	add    %al,(%rax)
  203d0e:	00 00                	add    %al,(%rax)
  203d10:	01 00                	add    %eax,(%rax)
  203d12:	00 00                	add    %al,(%rax)
  203d14:	00 00                	add    %al,(%rax)
  203d16:	00 00                	add    %al,(%rax)
  203d18:	0c 00                	or     $0x0,%al
  203d1a:	00 00                	add    %al,(%rax)
  203d1c:	00 00                	add    %al,(%rax)
  203d1e:	00 00                	add    %al,(%rax)
  203d20:	d0 0d 00 00 00 00    	rorb   0x0(%rip)        # 203d26 <_DYNAMIC+0x1e>
  203d26:	00 00                	add    %al,(%rax)
  203d28:	0d 00 00 00 00       	or     $0x0,%eax
  203d2d:	00 00                	add    %al,(%rax)
  203d2f:	00 84 26 00 00 00 00 	add    %al,0x0(%rsi,%riz,1)
  203d36:	00 00                	add    %al,(%rax)
  203d38:	19 00                	sbb    %eax,(%rax)
  203d3a:	00 00                	add    %al,(%rax)
  203d3c:	00 00                	add    %al,(%rax)
  203d3e:	00 00                	add    %al,(%rax)
  203d40:	f8                   	clc    
  203d41:	3c 20                	cmp    $0x20,%al
  203d43:	00 00                	add    %al,(%rax)
  203d45:	00 00                	add    %al,(%rax)
  203d47:	00 1b                	add    %bl,(%rbx)
  203d49:	00 00                	add    %al,(%rax)
  203d4b:	00 00                	add    %al,(%rax)
  203d4d:	00 00                	add    %al,(%rax)
  203d4f:	00 08                	add    %cl,(%rax)
  203d51:	00 00                	add    %al,(%rax)
  203d53:	00 00                	add    %al,(%rax)
  203d55:	00 00                	add    %al,(%rax)
  203d57:	00 1a                	add    %bl,(%rdx)
	...
  203d61:	3d 20 00 00 00       	cmp    $0x20,%eax
  203d66:	00 00                	add    %al,(%rax)
  203d68:	1c 00                	sbb    $0x0,%al
  203d6a:	00 00                	add    %al,(%rax)
  203d6c:	00 00                	add    %al,(%rax)
  203d6e:	00 00                	add    %al,(%rax)
  203d70:	08 00                	or     %al,(%rax)
  203d72:	00 00                	add    %al,(%rax)
  203d74:	00 00                	add    %al,(%rax)
  203d76:	00 00                	add    %al,(%rax)
  203d78:	f5                   	cmc    
  203d79:	fe                   	(bad)  
  203d7a:	ff 6f 00             	ljmp   *0x0(%rdi)
  203d7d:	00 00                	add    %al,(%rax)
  203d7f:	00 98 02 00 00 00    	add    %bl,0x2(%rax)
  203d85:	00 00                	add    %al,(%rax)
  203d87:	00 05 00 00 00 00    	add    %al,0x0(%rip)        # 203d8d <_DYNAMIC+0x85>
  203d8d:	00 00                	add    %al,(%rax)
  203d8f:	00 f8                	add    %bh,%al
  203d91:	05 00 00 00 00       	add    $0x0,%eax
  203d96:	00 00                	add    %al,(%rax)
  203d98:	06                   	(bad)  
  203d99:	00 00                	add    %al,(%rax)
  203d9b:	00 00                	add    %al,(%rax)
  203d9d:	00 00                	add    %al,(%rax)
  203d9f:	00 c8                	add    %cl,%al
  203da1:	02 00                	add    (%rax),%al
  203da3:	00 00                	add    %al,(%rax)
  203da5:	00 00                	add    %al,(%rax)
  203da7:	00 0a                	add    %cl,(%rdx)
  203da9:	00 00                	add    %al,(%rax)
  203dab:	00 00                	add    %al,(%rax)
  203dad:	00 00                	add    %al,(%rax)
  203daf:	00 a5 01 00 00 00    	add    %ah,0x1(%rbp)
  203db5:	00 00                	add    %al,(%rax)
  203db7:	00 0b                	add    %cl,(%rbx)
  203db9:	00 00                	add    %al,(%rax)
  203dbb:	00 00                	add    %al,(%rax)
  203dbd:	00 00                	add    %al,(%rax)
  203dbf:	00 18                	add    %bl,(%rax)
  203dc1:	00 00                	add    %al,(%rax)
  203dc3:	00 00                	add    %al,(%rax)
  203dc5:	00 00                	add    %al,(%rax)
  203dc7:	00 15 00 00 00 00    	add    %dl,0x0(%rip)        # 203dcd <_DYNAMIC+0xc5>
	...
  203dd5:	00 00                	add    %al,(%rax)
  203dd7:	00 03                	add    %al,(%rbx)
  203dd9:	00 00                	add    %al,(%rax)
  203ddb:	00 00                	add    %al,(%rax)
  203ddd:	00 00                	add    %al,(%rax)
  203ddf:	00 f8                	add    %bh,%al
  203de1:	3e 20 00             	and    %al,%ds:(%rax)
  203de4:	00 00                	add    %al,(%rax)
  203de6:	00 00                	add    %al,(%rax)
  203de8:	02 00                	add    (%rax),%al
  203dea:	00 00                	add    %al,(%rax)
  203dec:	00 00                	add    %al,(%rax)
  203dee:	00 00                	add    %al,(%rax)
  203df0:	58                   	pop    %rax
  203df1:	02 00                	add    (%rax),%al
  203df3:	00 00                	add    %al,(%rax)
  203df5:	00 00                	add    %al,(%rax)
  203df7:	00 14 00             	add    %dl,(%rax,%rax,1)
  203dfa:	00 00                	add    %al,(%rax)
  203dfc:	00 00                	add    %al,(%rax)
  203dfe:	00 00                	add    %al,(%rax)
  203e00:	07                   	(bad)  
  203e01:	00 00                	add    %al,(%rax)
  203e03:	00 00                	add    %al,(%rax)
  203e05:	00 00                	add    %al,(%rax)
  203e07:	00 17                	add    %dl,(%rdi)
  203e09:	00 00                	add    %al,(%rax)
  203e0b:	00 00                	add    %al,(%rax)
  203e0d:	00 00                	add    %al,(%rax)
  203e0f:	00 78 0b             	add    %bh,0xb(%rax)
  203e12:	00 00                	add    %al,(%rax)
  203e14:	00 00                	add    %al,(%rax)
  203e16:	00 00                	add    %al,(%rax)
  203e18:	07                   	(bad)  
  203e19:	00 00                	add    %al,(%rax)
  203e1b:	00 00                	add    %al,(%rax)
  203e1d:	00 00                	add    %al,(%rax)
  203e1f:	00 48 08             	add    %cl,0x8(%rax)
  203e22:	00 00                	add    %al,(%rax)
  203e24:	00 00                	add    %al,(%rax)
  203e26:	00 00                	add    %al,(%rax)
  203e28:	08 00                	or     %al,(%rax)
  203e2a:	00 00                	add    %al,(%rax)
  203e2c:	00 00                	add    %al,(%rax)
  203e2e:	00 00                	add    %al,(%rax)
  203e30:	30 03                	xor    %al,(%rbx)
  203e32:	00 00                	add    %al,(%rax)
  203e34:	00 00                	add    %al,(%rax)
  203e36:	00 00                	add    %al,(%rax)
  203e38:	09 00                	or     %eax,(%rax)
  203e3a:	00 00                	add    %al,(%rax)
  203e3c:	00 00                	add    %al,(%rax)
  203e3e:	00 00                	add    %al,(%rax)
  203e40:	18 00                	sbb    %al,(%rax)
  203e42:	00 00                	add    %al,(%rax)
  203e44:	00 00                	add    %al,(%rax)
  203e46:	00 00                	add    %al,(%rax)
  203e48:	1e                   	(bad)  
  203e49:	00 00                	add    %al,(%rax)
  203e4b:	00 00                	add    %al,(%rax)
  203e4d:	00 00                	add    %al,(%rax)
  203e4f:	00 08                	add    %cl,(%rax)
  203e51:	00 00                	add    %al,(%rax)
  203e53:	00 00                	add    %al,(%rax)
  203e55:	00 00                	add    %al,(%rax)
  203e57:	00 fb                	add    %bh,%bl
  203e59:	ff                   	(bad)  
  203e5a:	ff 6f 00             	ljmp   *0x0(%rdi)
  203e5d:	00 00                	add    %al,(%rax)
  203e5f:	00 01                	add    %al,(%rcx)
  203e61:	00 00                	add    %al,(%rax)
  203e63:	08 00                	or     %al,(%rax)
  203e65:	00 00                	add    %al,(%rax)
  203e67:	00 fe                	add    %bh,%dh
  203e69:	ff                   	(bad)  
  203e6a:	ff 6f 00             	ljmp   *0x0(%rdi)
  203e6d:	00 00                	add    %al,(%rax)
  203e6f:	00 e8                	add    %ch,%al
  203e71:	07                   	(bad)  
  203e72:	00 00                	add    %al,(%rax)
  203e74:	00 00                	add    %al,(%rax)
  203e76:	00 00                	add    %al,(%rax)
  203e78:	ff                   	(bad)  
  203e79:	ff                   	(bad)  
  203e7a:	ff 6f 00             	ljmp   *0x0(%rdi)
  203e7d:	00 00                	add    %al,(%rax)
  203e7f:	00 01                	add    %al,(%rcx)
  203e81:	00 00                	add    %al,(%rax)
  203e83:	00 00                	add    %al,(%rax)
  203e85:	00 00                	add    %al,(%rax)
  203e87:	00 f0                	add    %dh,%al
  203e89:	ff                   	(bad)  
  203e8a:	ff 6f 00             	ljmp   *0x0(%rdi)
  203e8d:	00 00                	add    %al,(%rax)
  203e8f:	00 9e 07 00 00 00    	add    %bl,0x7(%rsi)
  203e95:	00 00                	add    %al,(%rax)
  203e97:	00 f9                	add    %bh,%cl
  203e99:	ff                   	(bad)  
  203e9a:	ff 6f 00             	ljmp   *0x0(%rdi)
  203e9d:	00 00                	add    %al,(%rax)
  203e9f:	00 1a                	add    %bl,(%rdx)
	...

Disassembly of section .got:

0000000000203ef8 <_GLOBAL_OFFSET_TABLE_>:
  203ef8:	08 3d 20 00 00 00    	or     %bh,0x20(%rip)        # 203f1e <_GLOBAL_OFFSET_TABLE_+0x26>
	...
  203f0e:	00 00                	add    %al,(%rax)
  203f10:	06                   	(bad)  
  203f11:	0e                   	(bad)  
  203f12:	00 00                	add    %al,(%rax)
  203f14:	00 00                	add    %al,(%rax)
  203f16:	00 00                	add    %al,(%rax)
  203f18:	16                   	(bad)  
  203f19:	0e                   	(bad)  
  203f1a:	00 00                	add    %al,(%rax)
  203f1c:	00 00                	add    %al,(%rax)
  203f1e:	00 00                	add    %al,(%rax)
  203f20:	26 0e                	es (bad) 
  203f22:	00 00                	add    %al,(%rax)
  203f24:	00 00                	add    %al,(%rax)
  203f26:	00 00                	add    %al,(%rax)
  203f28:	36 0e                	ss (bad) 
  203f2a:	00 00                	add    %al,(%rax)
  203f2c:	00 00                	add    %al,(%rax)
  203f2e:	00 00                	add    %al,(%rax)
  203f30:	46 0e                	rex.RX (bad) 
  203f32:	00 00                	add    %al,(%rax)
  203f34:	00 00                	add    %al,(%rax)
  203f36:	00 00                	add    %al,(%rax)
  203f38:	56                   	push   %rsi
  203f39:	0e                   	(bad)  
  203f3a:	00 00                	add    %al,(%rax)
  203f3c:	00 00                	add    %al,(%rax)
  203f3e:	00 00                	add    %al,(%rax)
  203f40:	66 0e                	data16 (bad) 
  203f42:	00 00                	add    %al,(%rax)
  203f44:	00 00                	add    %al,(%rax)
  203f46:	00 00                	add    %al,(%rax)
  203f48:	76 0e                	jbe    203f58 <_GLOBAL_OFFSET_TABLE_+0x60>
  203f4a:	00 00                	add    %al,(%rax)
  203f4c:	00 00                	add    %al,(%rax)
  203f4e:	00 00                	add    %al,(%rax)
  203f50:	86 0e                	xchg   %cl,(%rsi)
  203f52:	00 00                	add    %al,(%rax)
  203f54:	00 00                	add    %al,(%rax)
  203f56:	00 00                	add    %al,(%rax)
  203f58:	96                   	xchg   %eax,%esi
  203f59:	0e                   	(bad)  
  203f5a:	00 00                	add    %al,(%rax)
  203f5c:	00 00                	add    %al,(%rax)
  203f5e:	00 00                	add    %al,(%rax)
  203f60:	a6                   	cmpsb  %es:(%rdi),%ds:(%rsi)
  203f61:	0e                   	(bad)  
  203f62:	00 00                	add    %al,(%rax)
  203f64:	00 00                	add    %al,(%rax)
  203f66:	00 00                	add    %al,(%rax)
  203f68:	b6 0e                	mov    $0xe,%dh
  203f6a:	00 00                	add    %al,(%rax)
  203f6c:	00 00                	add    %al,(%rax)
  203f6e:	00 00                	add    %al,(%rax)
  203f70:	c6                   	(bad)  
  203f71:	0e                   	(bad)  
  203f72:	00 00                	add    %al,(%rax)
  203f74:	00 00                	add    %al,(%rax)
  203f76:	00 00                	add    %al,(%rax)
  203f78:	d6                   	(bad)  
  203f79:	0e                   	(bad)  
  203f7a:	00 00                	add    %al,(%rax)
  203f7c:	00 00                	add    %al,(%rax)
  203f7e:	00 00                	add    %al,(%rax)
  203f80:	e6 0e                	out    %al,$0xe
  203f82:	00 00                	add    %al,(%rax)
  203f84:	00 00                	add    %al,(%rax)
  203f86:	00 00                	add    %al,(%rax)
  203f88:	f6 0e 00             	testb  $0x0,(%rsi)
  203f8b:	00 00                	add    %al,(%rax)
  203f8d:	00 00                	add    %al,(%rax)
  203f8f:	00 06                	add    %al,(%rsi)
  203f91:	0f 00 00             	sldt   (%rax)
  203f94:	00 00                	add    %al,(%rax)
  203f96:	00 00                	add    %al,(%rax)
  203f98:	16                   	(bad)  
  203f99:	0f 00 00             	sldt   (%rax)
  203f9c:	00 00                	add    %al,(%rax)
  203f9e:	00 00                	add    %al,(%rax)
  203fa0:	26 0f 00 00          	sldt   %es:(%rax)
  203fa4:	00 00                	add    %al,(%rax)
  203fa6:	00 00                	add    %al,(%rax)
  203fa8:	36 0f 00 00          	sldt   %ss:(%rax)
  203fac:	00 00                	add    %al,(%rax)
  203fae:	00 00                	add    %al,(%rax)
  203fb0:	46 0f 00 00          	rex.RX sldt (%rax)
  203fb4:	00 00                	add    %al,(%rax)
  203fb6:	00 00                	add    %al,(%rax)
  203fb8:	56                   	push   %rsi
  203fb9:	0f 00 00             	sldt   (%rax)
  203fbc:	00 00                	add    %al,(%rax)
  203fbe:	00 00                	add    %al,(%rax)
  203fc0:	66 0f 00 00          	data16 sldt (%rax)
  203fc4:	00 00                	add    %al,(%rax)
  203fc6:	00 00                	add    %al,(%rax)
  203fc8:	76 0f                	jbe    203fd9 <_GLOBAL_OFFSET_TABLE_+0xe1>
  203fca:	00 00                	add    %al,(%rax)
  203fcc:	00 00                	add    %al,(%rax)
  203fce:	00 00                	add    %al,(%rax)
  203fd0:	86 0f                	xchg   %cl,(%rdi)
	...

Disassembly of section .data:

0000000000204000 <__data_start>:
	...

0000000000204008 <__dso_handle>:
  204008:	08 40 20             	or     %al,0x20(%rax)
  20400b:	00 00                	add    %al,(%rax)
  20400d:	00 00                	add    %al,(%rax)
	...

0000000000204010 <n45>:
  204010:	28 00                	sub    %al,(%rax)
	...

0000000000204030 <n41>:
  204030:	01 00                	add    %eax,(%rax)
	...

0000000000204050 <n47>:
  204050:	63 00                	movslq (%rax),%eax
	...

0000000000204070 <n44>:
  204070:	23 00                	and    (%rax),%eax
	...

0000000000204090 <n42>:
  204090:	07                   	(bad)  
	...

00000000002040b0 <n43>:
  2040b0:	14 00                	adc    $0x0,%al
	...

00000000002040d0 <n46>:
  2040d0:	2f                   	(bad)  
	...

00000000002040f0 <n48>:
  2040f0:	e9 03 00 00 00       	jmpq   2040f8 <n48+0x8>
	...

0000000000204110 <node6>:
  204110:	9f                   	lahf   
  204111:	00 00                	add    %al,(%rax)
  204113:	00 06                	add    %al,(%rsi)
	...

0000000000204120 <n1>:
  204120:	24 00                	and    $0x0,%al
  204122:	00 00                	add    %al,(%rax)
  204124:	00 00                	add    %al,(%rax)
  204126:	00 00                	add    %al,(%rax)
  204128:	40                   	rex
  204129:	41 20 00             	and    %al,(%r8)
  20412c:	00 00                	add    %al,(%rax)
  20412e:	00 00                	add    %al,(%rax)
  204130:	60                   	(bad)  
  204131:	41 20 00             	and    %al,(%r8)
	...

0000000000204140 <n21>:
  204140:	08 00                	or     %al,(%rax)
  204142:	00 00                	add    %al,(%rax)
  204144:	00 00                	add    %al,(%rax)
  204146:	00 00                	add    %al,(%rax)
  204148:	c0 41 20 00          	rolb   $0x0,0x20(%rcx)
  20414c:	00 00                	add    %al,(%rax)
  20414e:	00 00                	add    %al,(%rax)
  204150:	80 41 20 00          	addb   $0x0,0x20(%rcx)
	...

0000000000204160 <n22>:
  204160:	32 00                	xor    (%rax),%al
  204162:	00 00                	add    %al,(%rax)
  204164:	00 00                	add    %al,(%rax)
  204166:	00 00                	add    %al,(%rax)
  204168:	a0 41 20 00 00 00 00 	movabs 0xe000000000002041,%al
  20416f:	00 e0 
  204171:	41 20 00             	and    %al,(%r8)
	...

0000000000204180 <n32>:
  204180:	16                   	(bad)  
  204181:	00 00                	add    %al,(%rax)
  204183:	00 00                	add    %al,(%rax)
  204185:	00 00                	add    %al,(%rax)
  204187:	00 b0 40 20 00 00    	add    %dh,0x2040(%rax)
  20418d:	00 00                	add    %al,(%rax)
  20418f:	00 70 40             	add    %dh,0x40(%rax)
  204192:	20 00                	and    %al,(%rax)
	...

00000000002041a0 <n33>:
  2041a0:	2d 00 00 00 00       	sub    $0x0,%eax
  2041a5:	00 00                	add    %al,(%rax)
  2041a7:	00 10                	add    %dl,(%rax)
  2041a9:	40 20 00             	and    %al,(%rax)
  2041ac:	00 00                	add    %al,(%rax)
  2041ae:	00 00                	add    %al,(%rax)
  2041b0:	d0 40 20             	rolb   0x20(%rax)
	...

00000000002041c0 <n31>:
  2041c0:	06                   	(bad)  
  2041c1:	00 00                	add    %al,(%rax)
  2041c3:	00 00                	add    %al,(%rax)
  2041c5:	00 00                	add    %al,(%rax)
  2041c7:	00 30                	add    %dh,(%rax)
  2041c9:	40 20 00             	and    %al,(%rax)
  2041cc:	00 00                	add    %al,(%rax)
  2041ce:	00 00                	add    %al,(%rax)
  2041d0:	90                   	nop
  2041d1:	40 20 00             	and    %al,(%rax)
	...

00000000002041e0 <n34>:
  2041e0:	6b 00 00             	imul   $0x0,(%rax),%eax
  2041e3:	00 00                	add    %al,(%rax)
  2041e5:	00 00                	add    %al,(%rax)
  2041e7:	00 50 40             	add    %dl,0x40(%rax)
  2041ea:	20 00                	and    %al,(%rax)
  2041ec:	00 00                	add    %al,(%rax)
  2041ee:	00 00                	add    %al,(%rax)
  2041f0:	f0 40 20 00          	lock and %al,(%rax)
	...

0000000000204200 <node1>:
  204200:	42 01 00             	rex.X add %eax,(%rax)
  204203:	00 01                	add    %al,(%rcx)
  204205:	00 00                	add    %al,(%rax)
  204207:	00 10                	add    %dl,(%rax)
  204209:	42 20 00             	rex.X and %al,(%rax)
  20420c:	00 00                	add    %al,(%rax)
	...

0000000000204210 <node2>:
  204210:	ac                   	lods   %ds:(%rsi),%al
  204211:	01 00                	add    %eax,(%rax)
  204213:	00 02                	add    %al,(%rdx)
  204215:	00 00                	add    %al,(%rax)
  204217:	00 20                	add    %ah,(%rax)
  204219:	42 20 00             	rex.X and %al,(%rax)
  20421c:	00 00                	add    %al,(%rax)
	...

0000000000204220 <node3>:
  204220:	7c 01                	jl     204223 <node3+0x3>
  204222:	00 00                	add    %al,(%rax)
  204224:	03 00                	add    (%rax),%eax
  204226:	00 00                	add    %al,(%rax)
  204228:	30 42 20             	xor    %al,0x20(%rdx)
  20422b:	00 00                	add    %al,(%rax)
  20422d:	00 00                	add    %al,(%rax)
	...

0000000000204230 <node4>:
  204230:	1c 01                	sbb    $0x1,%al
  204232:	00 00                	add    %al,(%rax)
  204234:	04 00                	add    $0x0,%al
  204236:	00 00                	add    %al,(%rax)
  204238:	40                   	rex
  204239:	42 20 00             	rex.X and %al,(%rax)
  20423c:	00 00                	add    %al,(%rax)
	...

0000000000204240 <node5>:
  204240:	cf                   	iret   
  204241:	01 00                	add    %eax,(%rax)
  204243:	00 05 00 00 00 10    	add    %al,0x10000000(%rip)        # 10204249 <_end+0xffff519>
  204249:	41 20 00             	and    %al,(%r8)
	...

0000000000204260 <host_table>:
  204260:	29 2a                	sub    %ebp,(%rdx)
  204262:	00 00                	add    %al,(%rax)
  204264:	00 00                	add    %al,(%rax)
  204266:	00 00                	add    %al,(%rax)
  204268:	43 2a 00             	rex.XB sub (%r8),%al
  20426b:	00 00                	add    %al,(%rax)
  20426d:	00 00                	add    %al,(%rax)
  20426f:	00 5d 2a             	add    %bl,0x2a(%rbp)
  204272:	00 00                	add    %al,(%rax)
  204274:	00 00                	add    %al,(%rax)
  204276:	00 00                	add    %al,(%rax)
  204278:	76 2a                	jbe    2042a4 <host_table+0x44>
	...

Disassembly of section .bss:

0000000000204660 <stdout@@GLIBC_2.2.5>:
	...

0000000000204670 <stdin@@GLIBC_2.2.5>:
	...

0000000000204680 <stderr@@GLIBC_2.2.5>:
	...

0000000000204688 <completed.7698>:
  204688:	00 00                	add    %al,(%rax)
	...

000000000020468c <bomb_id>:
  20468c:	00 00                	add    %al,(%rax)
	...

0000000000204690 <num_input_strings>:
	...

0000000000204698 <infile>:
	...

00000000002046a0 <input_strings>:
	...

0000000000204ce0 <scratch>:
	...

Disassembly of section .comment:

0000000000000000 <.comment>:
   0:	47                   	rex.RXB
   1:	43                   	rex.XB
   2:	43 3a 20             	rex.XB cmp (%r8),%spl
   5:	28 55 62             	sub    %dl,0x62(%rbp)
   8:	75 6e                	jne    78 <_init-0xd58>
   a:	74 75                	je     81 <_init-0xd4f>
   c:	20 37                	and    %dh,(%rdi)
   e:	2e 35 2e 30 2d 33    	cs xor $0x332d302e,%eax
  14:	75 62                	jne    78 <_init-0xd58>
  16:	75 6e                	jne    86 <_init-0xd4a>
  18:	74 75                	je     8f <_init-0xd41>
  1a:	31 7e 31             	xor    %edi,0x31(%rsi)
  1d:	38 2e                	cmp    %ch,(%rsi)
  1f:	30 34 29             	xor    %dh,(%rcx,%rbp,1)
  22:	20 37                	and    %dh,(%rdi)
  24:	2e                   	cs
  25:	35                   	.byte 0x35
  26:	2e 30 00             	xor    %al,%cs:(%rax)

Disassembly of section .debug_aranges:

0000000000000000 <.debug_aranges>:
   0:	2c 00                	sub    $0x0,%al
   2:	00 00                	add    %al,(%rax)
   4:	02 00                	add    (%rax),%al
   6:	00 00                	add    %al,(%rax)
   8:	00 00                	add    %al,(%rax)
   a:	08 00                	or     %al,(%rax)
   c:	00 00                	add    %al,(%rax)
   e:	00 00                	add    %al,(%rax)
  10:	aa                   	stos   %al,%es:(%rdi)
  11:	10 00                	adc    %al,(%rax)
  13:	00 00                	add    %al,(%rax)
  15:	00 00                	add    %al,(%rax)
  17:	00 5a 01             	add    %bl,0x1(%rdx)
	...

Disassembly of section .debug_info:

0000000000000000 <.debug_info>:
   0:	e7 08                	out    %eax,$0x8
   2:	00 00                	add    %al,(%rax)
   4:	04 00                	add    $0x0,%al
   6:	00 00                	add    %al,(%rax)
   8:	00 00                	add    %al,(%rax)
   a:	08 01                	or     %al,(%rcx)
   c:	dd 02                	fldl   (%rdx)
   e:	00 00                	add    %al,(%rax)
  10:	0c 06                	or     $0x6,%al
  12:	02 00                	add    (%rax),%al
  14:	00 3e                	add    %bh,(%rsi)
  16:	02 00                	add    (%rax),%al
  18:	00 aa 10 00 00 00    	add    %ch,0x10(%rdx)
  1e:	00 00                	add    %al,(%rax)
  20:	00 5a 01             	add    %bl,0x1(%rdx)
	...
  2b:	00 00                	add    %al,(%rax)
  2d:	02 23                	add    (%rbx),%ah
  2f:	00 00                	add    %al,(%rax)
  31:	00 03                	add    %al,(%rbx)
  33:	d8 38                	fdivrs (%rax)
  35:	00 00                	add    %al,(%rax)
  37:	00 03                	add    %al,(%rbx)
  39:	08 07                	or     %al,(%rdi)
  3b:	56                   	push   %rsi
  3c:	00 00                	add    %al,(%rax)
  3e:	00 03                	add    %al,(%rbx)
  40:	01 08                	add    %ecx,(%rax)
  42:	ca 01 00             	lret   $0x1
  45:	00 03                	add    %al,(%rbx)
  47:	02 07                	add    (%rdi),%al
  49:	2e 01 00             	add    %eax,%cs:(%rax)
  4c:	00 03                	add    %al,(%rbx)
  4e:	04 07                	add    $0x7,%al
  50:	5b                   	pop    %rbx
  51:	00 00                	add    %al,(%rax)
  53:	00 03                	add    %al,(%rbx)
  55:	01 06                	add    %eax,(%rsi)
  57:	cc                   	int3   
  58:	01 00                	add    %eax,(%rax)
  5a:	00 03                	add    %al,(%rbx)
  5c:	02 05 d8 01 00 00    	add    0x1d8(%rip),%al        # 23a <_init-0xb96>
  62:	04 04                	add    $0x4,%al
  64:	05 69 6e 74 00       	add    $0x746e69,%eax
  69:	03 08                	add    (%rax),%ecx
  6b:	05 77 00 00 00       	add    $0x77,%eax
  70:	02 07                	add    (%rdi),%al
  72:	00 00                	add    %al,(%rax)
  74:	00 04 8c             	add    %al,(%rsp,%rcx,4)
  77:	69 00 00 00 02 19    	imul   $0x19020000,(%rax),%eax
  7d:	02 00                	add    (%rax),%al
  7f:	00 04 8d 69 00 00 00 	add    %al,0x69(,%rcx,4)
  86:	05 08 06 08 8e       	add    $0x8e080608,%eax
  8b:	00 00                	add    %al,(%rax)
  8d:	00 03                	add    %al,(%rbx)
  8f:	01 06                	add    %eax,(%rsi)
  91:	d3 01                	roll   %cl,(%rcx)
  93:	00 00                	add    %al,(%rax)
  95:	07                   	(bad)  
  96:	8e 00                	mov    (%rax),%es
  98:	00 00                	add    %al,(%rax)
  9a:	08 a1 01 00 00 d8    	or     %ah,-0x27ffffff(%rcx)
  a0:	05 f5 1a 02 00       	add    $0x21af5,%eax
  a5:	00 09                	add    %cl,(%rcx)
  a7:	a3 00 00 00 05 f6 62 	movabs %eax,0x62f605000000
  ae:	00 00 
  b0:	00 00                	add    %al,(%rax)
  b2:	09 0f                	or     %ecx,(%rdi)
  b4:	00 00                	add    %al,(%rax)
  b6:	00 05 fb 88 00 00    	add    %al,0x88fb(%rip)        # 89b7 <__FRAME_END__+0x580b>
  bc:	00 08                	add    %cl,(%rax)
  be:	09 96 00 00 00 05    	or     %edx,0x5000000(%rsi)
  c4:	fc                   	cld    
  c5:	88 00                	mov    %al,(%rax)
  c7:	00 00                	add    %al,(%rax)
  c9:	10 09                	adc    %cl,(%rcx)
  cb:	23 02                	and    (%rdx),%eax
  cd:	00 00                	add    %al,(%rax)
  cf:	05 fd 88 00 00       	add    $0x88fd,%eax
  d4:	00 18                	add    %bl,(%rax)
  d6:	09 30                	or     %esi,(%rax)
  d8:	03 00                	add    (%rax),%eax
  da:	00 05 fe 88 00 00    	add    %al,0x88fe(%rip)        # 89de <__FRAME_END__+0x5832>
  e0:	00 20                	add    %ah,(%rax)
  e2:	09 11                	or     %edx,(%rcx)
  e4:	01 00                	add    %eax,(%rax)
  e6:	00 05 ff 88 00 00    	add    %al,0x88ff(%rip)        # 89eb <__FRAME_END__+0x583f>
  ec:	00 28                	add    %ch,(%rax)
  ee:	0a 88 01 00 00 05    	or     0x5000001(%rax),%cl
  f4:	00 01                	add    %al,(%rcx)
  f6:	88 00                	mov    %al,(%rax)
  f8:	00 00                	add    %al,(%rax)
  fa:	30 0a                	xor    %cl,(%rdx)
  fc:	44 00 00             	add    %r8b,(%rax)
  ff:	00 05 01 01 88 00    	add    %al,0x880101(%rip)        # 880206 <_end+0x67b4d6>
 105:	00 00                	add    %al,(%rax)
 107:	38 0a                	cmp    %cl,(%rdx)
 109:	aa                   	stos   %al,%es:(%rdi)
 10a:	00 00                	add    %al,(%rax)
 10c:	00 05 02 01 88 00    	add    %al,0x880102(%rip)        # 880214 <_end+0x67b4e4>
 112:	00 00                	add    %al,(%rax)
 114:	40 0a 41 01          	or     0x1(%rcx),%al
 118:	00 00                	add    %al,(%rax)
 11a:	05 04 01 88 00       	add    $0x880104,%eax
 11f:	00 00                	add    %al,(%rax)
 121:	48 0a c8             	rex.W or %al,%cl
 124:	02 00                	add    (%rax),%al
 126:	00 05 05 01 88 00    	add    %al,0x880105(%rip)        # 880231 <_end+0x67b501>
 12c:	00 00                	add    %al,(%rax)
 12e:	50                   	push   %rax
 12f:	0a 31                	or     (%rcx),%dh
 131:	02 00                	add    (%rax),%al
 133:	00 05 06 01 88 00    	add    %al,0x880106(%rip)        # 88023f <_end+0x67b50f>
 139:	00 00                	add    %al,(%rax)
 13b:	58                   	pop    %rax
 13c:	0a c1                	or     %cl,%al
 13e:	01 00                	add    %eax,(%rax)
 140:	00 05 08 01 5d 02    	add    %al,0x25d0108(%rip)        # 25d024e <_end+0x23cb51e>
 146:	00 00                	add    %al,(%rax)
 148:	60                   	(bad)  
 149:	0a 1c 00             	or     (%rax,%rax,1),%bl
 14c:	00 00                	add    %al,(%rax)
 14e:	05 0a 01 63 02       	add    $0x263010a,%eax
 153:	00 00                	add    %al,(%rax)
 155:	68 0a 8e 00 00       	pushq  $0x8e0a
 15a:	00 05 0c 01 62 00    	add    %al,0x62010c(%rip)        # 62026c <_end+0x41b53c>
 160:	00 00                	add    %al,(%rax)
 162:	70 0a                	jo     16e <_init-0xc62>
 164:	55                   	push   %rbp
 165:	01 00                	add    %eax,(%rax)
 167:	00 05 10 01 62 00    	add    %al,0x620110(%rip)        # 62027d <_end+0x41b54d>
 16d:	00 00                	add    %al,(%rax)
 16f:	74 0a                	je     17b <_init-0xc55>
 171:	cf                   	iret   
 172:	00 00                	add    %al,(%rax)
 174:	00 05 12 01 70 00    	add    %al,0x700112(%rip)        # 70028c <_end+0x4fb55c>
 17a:	00 00                	add    %al,(%rax)
 17c:	78 0a                	js     188 <_init-0xc48>
 17e:	b6 00                	mov    $0x0,%dh
 180:	00 00                	add    %al,(%rax)
 182:	05 16 01 46 00       	add    $0x460116,%eax
 187:	00 00                	add    %al,(%rax)
 189:	80 0a e2             	orb    $0xe2,(%rdx)
 18c:	01 00                	add    %eax,(%rax)
 18e:	00 05 17 01 54 00    	add    %al,0x540117(%rip)        # 5402ab <_end+0x33b57b>
 194:	00 00                	add    %al,(%rax)
 196:	82                   	(bad)  
 197:	0a 2a                	or     (%rdx),%ch
 199:	00 00                	add    %al,(%rax)
 19b:	00 05 18 01 69 02    	add    %al,0x2690118(%rip)        # 26902b9 <_end+0x248b589>
 1a1:	00 00                	add    %al,(%rax)
 1a3:	83 0a 4f             	orl    $0x4f,(%rdx)
 1a6:	01 00                	add    %eax,(%rax)
 1a8:	00 05 1c 01 79 02    	add    %al,0x279011c(%rip)        # 27902ca <_end+0x258b59a>
 1ae:	00 00                	add    %al,(%rax)
 1b0:	88 0a                	mov    %cl,(%rdx)
 1b2:	d3 00                	roll   %cl,(%rax)
 1b4:	00 00                	add    %al,(%rax)
 1b6:	05 25 01 7b 00       	add    $0x7b0125,%eax
 1bb:	00 00                	add    %al,(%rax)
 1bd:	90                   	nop
 1be:	0a 60 02             	or     0x2(%rax),%ah
 1c1:	00 00                	add    %al,(%rax)
 1c3:	05 2d 01 86 00       	add    $0x86012d,%eax
 1c8:	00 00                	add    %al,(%rax)
 1ca:	98                   	cwtl   
 1cb:	0a 67 02             	or     0x2(%rdi),%ah
 1ce:	00 00                	add    %al,(%rax)
 1d0:	05 2e 01 86 00       	add    $0x86012e,%eax
 1d5:	00 00                	add    %al,(%rax)
 1d7:	a0 0a 6e 02 00 00 05 	movabs 0x12f050000026e0a,%al
 1de:	2f 01 
 1e0:	86 00                	xchg   %al,(%rax)
 1e2:	00 00                	add    %al,(%rax)
 1e4:	a8 0a                	test   $0xa,%al
 1e6:	75 02                	jne    1ea <_init-0xbe6>
 1e8:	00 00                	add    %al,(%rax)
 1ea:	05 30 01 86 00       	add    $0x860130,%eax
 1ef:	00 00                	add    %al,(%rax)
 1f1:	b0 0a                	mov    $0xa,%al
 1f3:	7c 02                	jl     1f7 <_init-0xbd9>
 1f5:	00 00                	add    %al,(%rax)
 1f7:	05 32 01 2d 00       	add    $0x2d0132,%eax
 1fc:	00 00                	add    %al,(%rax)
 1fe:	b8 0a 5d 01 00       	mov    $0x15d0a,%eax
 203:	00 05 33 01 62 00    	add    %al,0x620133(%rip)        # 62033c <_end+0x41b60c>
 209:	00 00                	add    %al,(%rax)
 20b:	c0 0a 83             	rorb   $0x83,(%rdx)
 20e:	02 00                	add    (%rax),%al
 210:	00 05 35 01 7f 02    	add    %al,0x27f0135(%rip)        # 27f034b <_end+0x25eb61b>
 216:	00 00                	add    %al,(%rax)
 218:	c4                   	(bad)  
 219:	00 02                	add    %al,(%rdx)
 21b:	a5                   	movsl  %ds:(%rsi),%es:(%rdi)
 21c:	01 00                	add    %eax,(%rax)
 21e:	00 06                	add    %al,(%rsi)
 220:	07                   	(bad)  
 221:	9a                   	(bad)  
 222:	00 00                	add    %al,(%rax)
 224:	00 0b                	add    %cl,(%rbx)
 226:	96                   	xchg   %eax,%esi
 227:	01 00                	add    %eax,(%rax)
 229:	00 05 9a 08 f2 00    	add    %al,0xf2089a(%rip)        # f20ac9 <_end+0xd1bd99>
 22f:	00 00                	add    %al,(%rax)
 231:	18 05 a0 5d 02 00    	sbb    %al,0x25da0(%rip)        # 25fd7 <__FRAME_END__+0x22e2b>
 237:	00 09                	add    %cl,(%rcx)
 239:	13 02                	adc    (%rdx),%eax
 23b:	00 00                	add    %al,(%rax)
 23d:	05 a1 5d 02 00       	add    $0x25da1,%eax
 242:	00 00                	add    %al,(%rax)
 244:	09 28                	or     %ebp,(%rax)
 246:	01 00                	add    %eax,(%rax)
 248:	00 05 a2 63 02 00    	add    %al,0x263a2(%rip)        # 265f0 <__FRAME_END__+0x23444>
 24e:	00 08                	add    %cl,(%rax)
 250:	09 b0 01 00 00 05    	or     %esi,0x5000001(%rax)
 256:	a6                   	cmpsb  %es:(%rdi),%ds:(%rsi)
 257:	62                   	(bad)  
 258:	00 00                	add    %al,(%rax)
 25a:	00 10                	add    %dl,(%rax)
 25c:	00 06                	add    %al,(%rsi)
 25e:	08 2c 02             	or     %ch,(%rdx,%rax,1)
 261:	00 00                	add    %al,(%rax)
 263:	06                   	(bad)  
 264:	08 9a 00 00 00 0c    	or     %bl,0xc000000(%rdx)
 26a:	8e 00                	mov    (%rax),%es
 26c:	00 00                	add    %al,(%rax)
 26e:	79 02                	jns    272 <_init-0xb5e>
 270:	00 00                	add    %al,(%rax)
 272:	0d 38 00 00 00       	or     $0x38,%eax
 277:	00 00                	add    %al,(%rax)
 279:	06                   	(bad)  
 27a:	08 25 02 00 00 0c    	or     %ah,0xc000002(%rip)        # c000282 <_end+0xbdfb552>
 280:	8e 00                	mov    (%rax),%es
 282:	00 00                	add    %al,(%rax)
 284:	8f 02                	popq   (%rdx)
 286:	00 00                	add    %al,(%rax)
 288:	0d 38 00 00 00       	or     $0x38,%eax
 28d:	13 00                	adc    (%rax),%eax
 28f:	0e                   	(bad)  
 290:	03 01                	add    (%rcx),%eax
 292:	00 00                	add    %al,(%rax)
 294:	0f 79 01             	vmwrite (%rcx),%rax
 297:	00 00                	add    %al,(%rax)
 299:	05 3f 01 8f 02       	add    $0x28f013f,%eax
 29e:	00 00                	add    %al,(%rax)
 2a0:	0f f1 01             	psllw  (%rcx),%mm0
 2a3:	00 00                	add    %al,(%rax)
 2a5:	05 40 01 8f 02       	add    $0x28f0140,%eax
 2aa:	00 00                	add    %al,(%rax)
 2ac:	0f 34                	sysenter 
 2ae:	00 00                	add    %al,(%rax)
 2b0:	00 05 41 01 8f 02    	add    %al,0x28f0141(%rip)        # 28f03f7 <_end+0x26eb6c7>
 2b6:	00 00                	add    %al,(%rax)
 2b8:	06                   	(bad)  
 2b9:	08 95 00 00 00 07    	or     %dl,0x7000000(%rbp)
 2bf:	b8 02 00 00 10       	mov    $0x10000002,%eax
 2c4:	b8 02 00 00 11       	mov    $0x11000002,%eax
 2c9:	fd                   	std    
 2ca:	00 00                	add    %al,(%rax)
 2cc:	00 07                	add    %al,(%rdi)
 2ce:	87 63 02             	xchg   %esp,0x2(%rbx)
 2d1:	00 00                	add    %al,(%rax)
 2d3:	11 72 01             	adc    %esi,0x1(%rdx)
 2d6:	00 00                	add    %al,(%rax)
 2d8:	07                   	(bad)  
 2d9:	88 63 02             	mov    %ah,0x2(%rbx)
 2dc:	00 00                	add    %al,(%rax)
 2de:	11 8c 02 00 00 07 89 	adc    %ecx,-0x76f90000(%rdx,%rax,1)
 2e5:	63 02                	movslq (%rdx),%eax
 2e7:	00 00                	add    %al,(%rax)
 2e9:	11 1f                	adc    %ebx,(%rdi)
 2eb:	01 00                	add    %eax,(%rax)
 2ed:	00 08                	add    %cl,(%rax)
 2ef:	1a 62 00             	sbb    0x0(%rdx),%ah
 2f2:	00 00                	add    %al,(%rax)
 2f4:	0c be                	or     $0xbe,%al
 2f6:	02 00                	add    (%rax),%al
 2f8:	00 ff                	add    %bh,%bh
 2fa:	02 00                	add    (%rax),%al
 2fc:	00 12                	add    %dl,(%rdx)
 2fe:	00 07                	add    %al,(%rdi)
 300:	f4                   	hlt    
 301:	02 00                	add    (%rax),%al
 303:	00 11                	add    %dl,(%rcx)
 305:	b5 01                	mov    $0x1,%ch
 307:	00 00                	add    %al,(%rax)
 309:	08 1b                	or     %bl,(%rbx)
 30b:	ff 02                	incl   (%rdx)
 30d:	00 00                	add    %al,(%rax)
 30f:	03 08                	add    (%rax),%ecx
 311:	05 72 00 00 00       	add    $0x72,%eax
 316:	03 08                	add    (%rax),%ecx
 318:	07                   	(bad)  
 319:	51                   	push   %rcx
 31a:	00 00                	add    %al,(%rax)
 31c:	00 13                	add    %dl,(%rbx)
 31e:	db 00                	fildl  (%rax)
 320:	00 00                	add    %al,(%rax)
 322:	01 22                	add    %esp,(%rdx)
 324:	32 03                	xor    (%rbx),%al
 326:	00 00                	add    %al,(%rax)
 328:	09 03                	or     %eax,(%rbx)
 32a:	98                   	cwtl   
 32b:	46 20 00             	rex.RX and %r8b,(%rax)
 32e:	00 00                	add    %al,(%rax)
 330:	00 00                	add    %al,(%rax)
 332:	06                   	(bad)  
 333:	08 1a                	or     %bl,(%rdx)
 335:	02 00                	add    (%rax),%al
 337:	00 14 2b             	add    %dl,(%rbx,%rbp,1)
 33a:	03 00                	add    (%rax),%eax
 33c:	00 01                	add    %al,(%rcx)
 33e:	24 62                	and    $0x62,%al
 340:	00 00                	add    %al,(%rax)
 342:	00 aa 10 00 00 00    	add    %ch,0x10(%rdx)
 348:	00 00                	add    %al,(%rax)
 34a:	00 5a 01             	add    %bl,0x1(%rdx)
 34d:	00 00                	add    %al,(%rax)
 34f:	00 00                	add    %al,(%rax)
 351:	00 00                	add    %al,(%rax)
 353:	01 9c 11 07 00 00 15 	add    %ebx,0x15000007(%rcx,%rdx,1)
 35a:	d8 02                	fadds  (%rdx)
 35c:	00 00                	add    %al,(%rax)
 35e:	01 24 62             	add    %esp,(%rdx,%riz,2)
 361:	00 00                	add    %al,(%rax)
 363:	00 00                	add    %al,(%rax)
 365:	00 00                	add    %al,(%rax)
 367:	00 15 93 02 00 00    	add    %dl,0x293(%rip)        # 600 <_init-0x7d0>
 36d:	01 24 11             	add    %esp,(%rcx,%rdx,1)
 370:	07                   	(bad)  
 371:	00 00                	add    %al,(%rax)
 373:	8b 00                	mov    (%rax),%eax
 375:	00 00                	add    %al,(%rax)
 377:	16                   	(bad)  
 378:	0d 02 00 00 01       	or     $0x1000002,%eax
 37d:	26 88 00             	mov    %al,%es:(%rax)
 380:	00 00                	add    %al,(%rax)
 382:	fd                   	std    
 383:	00 00                	add    %al,(%rax)
 385:	00 17                	add    %dl,(%rdi)
 387:	17                   	(bad)  
 388:	07                   	(bad)  
 389:	00 00                	add    %al,(%rax)
 38b:	e5 10                	in     $0x10,%eax
 38d:	00 00                	add    %al,(%rax)
 38f:	00 00                	add    %al,(%rax)
 391:	00 00                	add    %al,(%rax)
 393:	0c 00                	or     $0x0,%al
 395:	00 00                	add    %al,(%rax)
 397:	00 00                	add    %al,(%rax)
 399:	00 00                	add    %al,(%rax)
 39b:	01 45 c6             	add    %eax,-0x3a(%rbp)
 39e:	03 00                	add    (%rax),%eax
 3a0:	00 18                	add    %bl,(%rax)
 3a2:	27                   	(bad)  
 3a3:	07                   	(bad)  
 3a4:	00 00                	add    %al,(%rax)
 3a6:	7f 01                	jg     3a9 <_init-0xa27>
 3a8:	00 00                	add    %al,(%rax)
 3aa:	19 f1                	sbb    %esi,%ecx
 3ac:	10 00                	adc    %al,(%rax)
 3ae:	00 00                	add    %al,(%rax)
 3b0:	00 00                	add    %al,(%rax)
 3b2:	00 74 07 00          	add    %dh,0x0(%rdi,%rax,1)
 3b6:	00 1a                	add    %bl,(%rdx)
 3b8:	01 55 09             	add    %edx,0x9(%rbp)
 3bb:	03 28                	add    (%rax),%ebp
 3bd:	27                   	(bad)  
	...
 3c6:	17                   	(bad)  
 3c7:	17                   	(bad)  
 3c8:	07                   	(bad)  
 3c9:	00 00                	add    %al,(%rax)
 3cb:	f1                   	icebp  
 3cc:	10 00                	adc    %al,(%rax)
 3ce:	00 00                	add    %al,(%rax)
 3d0:	00 00                	add    %al,(%rax)
 3d2:	00 0c 00             	add    %cl,(%rax,%rax,1)
 3d5:	00 00                	add    %al,(%rax)
 3d7:	00 00                	add    %al,(%rax)
 3d9:	00 00                	add    %al,(%rax)
 3db:	01 46 06             	add    %eax,0x6(%rsi)
 3de:	04 00                	add    $0x0,%al
 3e0:	00 18                	add    %bl,(%rax)
 3e2:	27                   	(bad)  
 3e3:	07                   	(bad)  
 3e4:	00 00                	add    %al,(%rax)
 3e6:	a7                   	cmpsl  %es:(%rdi),%ds:(%rsi)
 3e7:	01 00                	add    %eax,(%rax)
 3e9:	00 19                	add    %bl,(%rcx)
 3eb:	fd                   	std    
 3ec:	10 00                	adc    %al,(%rax)
 3ee:	00 00                	add    %al,(%rax)
 3f0:	00 00                	add    %al,(%rax)
 3f2:	00 74 07 00          	add    %dh,0x0(%rdi,%rax,1)
 3f6:	00 1a                	add    %bl,(%rdx)
 3f8:	01 55 09             	add    %edx,0x9(%rbp)
 3fb:	03 68 27             	add    0x27(%rax),%ebp
	...
 406:	17                   	(bad)  
 407:	17                   	(bad)  
 408:	07                   	(bad)  
 409:	00 00                	add    %al,(%rax)
 40b:	0f 11 00             	movups %xmm0,(%rax)
 40e:	00 00                	add    %al,(%rax)
 410:	00 00                	add    %al,(%rax)
 412:	00 0c 00             	add    %cl,(%rax,%rax,1)
 415:	00 00                	add    %al,(%rax)
 417:	00 00                	add    %al,(%rax)
 419:	00 00                	add    %al,(%rax)
 41b:	01 4d 46             	add    %ecx,0x46(%rbp)
 41e:	04 00                	add    $0x0,%al
 420:	00 18                	add    %bl,(%rax)
 422:	27                   	(bad)  
 423:	07                   	(bad)  
 424:	00 00                	add    %al,(%rax)
 426:	cf                   	iret   
 427:	01 00                	add    %eax,(%rax)
 429:	00 19                	add    %bl,(%rcx)
 42b:	1b 11                	sbb    (%rcx),%edx
 42d:	00 00                	add    %al,(%rax)
 42f:	00 00                	add    %al,(%rax)
 431:	00 00                	add    %al,(%rax)
 433:	74 07                	je     43c <_init-0x994>
 435:	00 00                	add    %al,(%rax)
 437:	1a 01                	sbb    (%rcx),%al
 439:	55                   	push   %rbp
 43a:	09 03                	or     %eax,(%rbx)
 43c:	98                   	cwtl   
 43d:	27                   	(bad)  
	...
 446:	17                   	(bad)  
 447:	17                   	(bad)  
 448:	07                   	(bad)  
 449:	00 00                	add    %al,(%rax)
 44b:	2d 11 00 00 00       	sub    $0x11,%eax
 450:	00 00                	add    %al,(%rax)
 452:	00 0c 00             	add    %cl,(%rax,%rax,1)
 455:	00 00                	add    %al,(%rax)
 457:	00 00                	add    %al,(%rax)
 459:	00 00                	add    %al,(%rax)
 45b:	01 54 86 04          	add    %edx,0x4(%rsi,%rax,4)
 45f:	00 00                	add    %al,(%rax)
 461:	18 27                	sbb    %ah,(%rdi)
 463:	07                   	(bad)  
 464:	00 00                	add    %al,(%rax)
 466:	f7 01 00 00 19 39    	testl  $0x39190000,(%rcx)
 46c:	11 00                	adc    %eax,(%rax)
 46e:	00 00                	add    %al,(%rax)
 470:	00 00                	add    %al,(%rax)
 472:	00 74 07 00          	add    %dh,0x0(%rdi,%rax,1)
 476:	00 1a                	add    %bl,(%rdx)
 478:	01 55 09             	add    %edx,0x9(%rbp)
 47b:	03 dd                	add    %ebp,%ebx
 47d:	26 00 00             	add    %al,%es:(%rax)
 480:	00 00                	add    %al,(%rax)
 482:	00 00                	add    %al,(%rax)
 484:	00 00                	add    %al,(%rax)
 486:	17                   	(bad)  
 487:	17                   	(bad)  
 488:	07                   	(bad)  
 489:	00 00                	add    %al,(%rax)
 48b:	4b 11 00             	rex.WXB adc %rax,(%r8)
 48e:	00 00                	add    %al,(%rax)
 490:	00 00                	add    %al,(%rax)
 492:	00 0c 00             	add    %cl,(%rax,%rax,1)
 495:	00 00                	add    %al,(%rax)
 497:	00 00                	add    %al,(%rax)
 499:	00 00                	add    %al,(%rax)
 49b:	01 5b c6             	add    %ebx,-0x3a(%rbx)
 49e:	04 00                	add    $0x0,%al
 4a0:	00 18                	add    %bl,(%rax)
 4a2:	27                   	(bad)  
 4a3:	07                   	(bad)  
 4a4:	00 00                	add    %al,(%rax)
 4a6:	1f                   	(bad)  
 4a7:	02 00                	add    (%rax),%al
 4a9:	00 19                	add    %bl,(%rcx)
 4ab:	57                   	push   %rdi
 4ac:	11 00                	adc    %eax,(%rax)
 4ae:	00 00                	add    %al,(%rax)
 4b0:	00 00                	add    %al,(%rax)
 4b2:	00 74 07 00          	add    %dh,0x0(%rdi,%rax,1)
 4b6:	00 1a                	add    %bl,(%rdx)
 4b8:	01 55 09             	add    %edx,0x9(%rbp)
 4bb:	03 fb                	add    %ebx,%edi
 4bd:	26 00 00             	add    %al,%es:(%rax)
 4c0:	00 00                	add    %al,(%rax)
 4c2:	00 00                	add    %al,(%rax)
 4c4:	00 00                	add    %al,(%rax)
 4c6:	17                   	(bad)  
 4c7:	17                   	(bad)  
 4c8:	07                   	(bad)  
 4c9:	00 00                	add    %al,(%rax)
 4cb:	69 11 00 00 00 00    	imul   $0x0,(%rcx),%edx
 4d1:	00 00                	add    %al,(%rax)
 4d3:	0c 00                	or     $0x0,%al
 4d5:	00 00                	add    %al,(%rax)
 4d7:	00 00                	add    %al,(%rax)
 4d9:	00 00                	add    %al,(%rax)
 4db:	01 61 06             	add    %esp,0x6(%rcx)
 4de:	05 00 00 18 27       	add    $0x27180000,%eax
 4e3:	07                   	(bad)  
 4e4:	00 00                	add    %al,(%rax)
 4e6:	47 02 00             	rex.RXB add (%r8),%r8b
 4e9:	00 19                	add    %bl,(%rcx)
 4eb:	75 11                	jne    4fe <_init-0x8d2>
 4ed:	00 00                	add    %al,(%rax)
 4ef:	00 00                	add    %al,(%rax)
 4f1:	00 00                	add    %al,(%rax)
 4f3:	74 07                	je     4fc <_init-0x8d4>
 4f5:	00 00                	add    %al,(%rax)
 4f7:	1a 01                	sbb    (%rcx),%al
 4f9:	55                   	push   %rbp
 4fa:	09 03                	or     %eax,(%rbx)
 4fc:	c8 27 00 00          	enterq $0x27,$0x0
 500:	00 00                	add    %al,(%rax)
 502:	00 00                	add    %al,(%rax)
 504:	00 00                	add    %al,(%rax)
 506:	17                   	(bad)  
 507:	17                   	(bad)  
 508:	07                   	(bad)  
 509:	00 00                	add    %al,(%rax)
 50b:	87 11                	xchg   %edx,(%rcx)
 50d:	00 00                	add    %al,(%rax)
 50f:	00 00                	add    %al,(%rax)
 511:	00 00                	add    %al,(%rax)
 513:	0c 00                	or     $0x0,%al
 515:	00 00                	add    %al,(%rax)
 517:	00 00                	add    %al,(%rax)
 519:	00 00                	add    %al,(%rax)
 51b:	01 67 46             	add    %esp,0x46(%rdi)
 51e:	05 00 00 18 27       	add    $0x27180000,%eax
 523:	07                   	(bad)  
 524:	00 00                	add    %al,(%rax)
 526:	6f                   	outsl  %ds:(%rsi),(%dx)
 527:	02 00                	add    (%rax),%al
 529:	00 19                	add    %bl,(%rcx)
 52b:	93                   	xchg   %eax,%ebx
 52c:	11 00                	adc    %eax,(%rax)
 52e:	00 00                	add    %al,(%rax)
 530:	00 00                	add    %al,(%rax)
 532:	00 74 07 00          	add    %dh,0x0(%rdi,%rax,1)
 536:	00 1a                	add    %bl,(%rdx)
 538:	01 55 09             	add    %edx,0x9(%rbp)
 53b:	03 0a                	add    (%rdx),%ecx
 53d:	27                   	(bad)  
	...
 546:	17                   	(bad)  
 547:	17                   	(bad)  
 548:	07                   	(bad)  
 549:	00 00                	add    %al,(%rax)
 54b:	bf 11 00 00 00       	mov    $0x11,%edi
 550:	00 00                	add    %al,(%rax)
 552:	00 18                	add    %bl,(%rax)
 554:	00 00                	add    %al,(%rax)
 556:	00 00                	add    %al,(%rax)
 558:	00 00                	add    %al,(%rax)
 55a:	00 01                	add    %al,(%rcx)
 55c:	37                   	(bad)  
 55d:	8b 05 00 00 18 27    	mov    0x27180000(%rip),%eax        # 27180563 <_end+0x26f7b833>
 563:	07                   	(bad)  
 564:	00 00                	add    %al,(%rax)
 566:	97                   	xchg   %eax,%edi
 567:	02 00                	add    (%rax),%al
 569:	00 19                	add    %bl,(%rcx)
 56b:	d7                   	xlat   %ds:(%rbx)
 56c:	11 00                	adc    %eax,(%rax)
 56e:	00 00                	add    %al,(%rax)
 570:	00 00                	add    %al,(%rax)
 572:	00 65 08             	add    %ah,0x8(%rbp)
 575:	00 00                	add    %al,(%rax)
 577:	1a 01                	sbb    (%rcx),%al
 579:	55                   	push   %rbp
 57a:	01 31                	add    %esi,(%rcx)
 57c:	1a 01                	sbb    (%rcx),%al
 57e:	54                   	push   %rsp
 57f:	09 03                	or     %eax,(%rbx)
 581:	a6                   	cmpsb  %es:(%rdi),%ds:(%rsi)
 582:	26 00 00             	add    %al,%es:(%rax)
 585:	00 00                	add    %al,(%rax)
 587:	00 00                	add    %al,(%rax)
 589:	00 00                	add    %al,(%rax)
 58b:	17                   	(bad)  
 58c:	17                   	(bad)  
 58d:	07                   	(bad)  
 58e:	00 00                	add    %al,(%rax)
 590:	e1 11                	loope  5a3 <_init-0x82d>
 592:	00 00                	add    %al,(%rax)
 594:	00 00                	add    %al,(%rax)
 596:	00 00                	add    %al,(%rax)
 598:	19 00                	sbb    %eax,(%rax)
 59a:	00 00                	add    %al,(%rax)
 59c:	00 00                	add    %al,(%rax)
 59e:	00 00                	add    %al,(%rax)
 5a0:	01 3e                	add    %edi,(%rsi)
 5a2:	d0 05 00 00 18 27    	rolb   0x27180000(%rip)        # 271805a8 <_end+0x26f7b878>
 5a8:	07                   	(bad)  
 5a9:	00 00                	add    %al,(%rax)
 5ab:	c3                   	retq   
 5ac:	02 00                	add    (%rax),%al
 5ae:	00 19                	add    %bl,(%rcx)
 5b0:	fa                   	cli    
 5b1:	11 00                	adc    %eax,(%rax)
 5b3:	00 00                	add    %al,(%rax)
 5b5:	00 00                	add    %al,(%rax)
 5b7:	00 65 08             	add    %ah,0x8(%rbp)
 5ba:	00 00                	add    %al,(%rax)
 5bc:	1a 01                	sbb    (%rcx),%al
 5be:	55                   	push   %rbp
 5bf:	01 31                	add    %esi,(%rcx)
 5c1:	1a 01                	sbb    (%rcx),%al
 5c3:	54                   	push   %rsp
 5c4:	09 03                	or     %eax,(%rbx)
 5c6:	c3                   	retq   
 5c7:	26 00 00             	add    %al,%es:(%rax)
 5ca:	00 00                	add    %al,(%rax)
 5cc:	00 00                	add    %al,(%rax)
 5ce:	00 00                	add    %al,(%rax)
 5d0:	1b d0                	sbb    %eax,%edx
 5d2:	10 00                	adc    %al,(%rax)
 5d4:	00 00                	add    %al,(%rax)
 5d6:	00 00                	add    %al,(%rax)
 5d8:	00 70 08             	add    %dh,0x8(%rax)
 5db:	00 00                	add    %al,(%rax)
 5dd:	ef                   	out    %eax,(%dx)
 5de:	05 00 00 1a 01       	add    $0x11a0000,%eax
 5e3:	54                   	push   %rsp
 5e4:	09 03                	or     %eax,(%rbx)
 5e6:	a4                   	movsb  %ds:(%rsi),%es:(%rdi)
 5e7:	26 00 00             	add    %al,%es:(%rax)
 5ea:	00 00                	add    %al,(%rax)
 5ec:	00 00                	add    %al,(%rax)
 5ee:	00 1c e5 10 00 00 00 	add    %bl,0x10(,%riz,8)
 5f5:	00 00                	add    %al,(%rax)
 5f7:	00 7b 08             	add    %bh,0x8(%rbx)
 5fa:	00 00                	add    %al,(%rax)
 5fc:	1c 02                	sbb    $0x2,%al
 5fe:	11 00                	adc    %eax,(%rax)
 600:	00 00                	add    %al,(%rax)
 602:	00 00                	add    %al,(%rax)
 604:	00 86 08 00 00 1c    	add    %al,0x1c000008(%rsi)
 60a:	0a 11                	or     (%rcx),%dl
 60c:	00 00                	add    %al,(%rax)
 60e:	00 00                	add    %al,(%rax)
 610:	00 00                	add    %al,(%rax)
 612:	91                   	xchg   %eax,%ecx
 613:	08 00                	or     %al,(%rax)
 615:	00 1c 0f             	add    %bl,(%rdi,%rcx,1)
 618:	11 00                	adc    %eax,(%rax)
 61a:	00 00                	add    %al,(%rax)
 61c:	00 00                	add    %al,(%rax)
 61e:	00 9c 08 00 00 1c 20 	add    %bl,0x201c0000(%rax,%rcx,1)
 625:	11 00                	adc    %eax,(%rax)
 627:	00 00                	add    %al,(%rax)
 629:	00 00                	add    %al,(%rax)
 62b:	00 86 08 00 00 1c    	add    %al,0x1c000008(%rsi)
 631:	28 11                	sub    %dl,(%rcx)
 633:	00 00                	add    %al,(%rax)
 635:	00 00                	add    %al,(%rax)
 637:	00 00                	add    %al,(%rax)
 639:	a7                   	cmpsl  %es:(%rdi),%ds:(%rsi)
 63a:	08 00                	or     %al,(%rax)
 63c:	00 1c 2d 11 00 00 00 	add    %bl,0x11(,%rbp,1)
 643:	00 00                	add    %al,(%rax)
 645:	00 9c 08 00 00 1c 3e 	add    %bl,0x3e1c0000(%rax,%rcx,1)
 64c:	11 00                	adc    %eax,(%rax)
 64e:	00 00                	add    %al,(%rax)
 650:	00 00                	add    %al,(%rax)
 652:	00 86 08 00 00 1c    	add    %al,0x1c000008(%rsi)
 658:	46 11 00             	rex.RX adc %r8d,(%rax)
 65b:	00 00                	add    %al,(%rax)
 65d:	00 00                	add    %al,(%rax)
 65f:	00 b2 08 00 00 1c    	add    %dh,0x1c000008(%rdx)
 665:	4b 11 00             	rex.WXB adc %rax,(%r8)
 668:	00 00                	add    %al,(%rax)
 66a:	00 00                	add    %al,(%rax)
 66c:	00 9c 08 00 00 1c 5c 	add    %bl,0x5c1c0000(%rax,%rcx,1)
 673:	11 00                	adc    %eax,(%rax)
 675:	00 00                	add    %al,(%rax)
 677:	00 00                	add    %al,(%rax)
 679:	00 86 08 00 00 1c    	add    %al,0x1c000008(%rsi)
 67f:	64 11 00             	adc    %eax,%fs:(%rax)
 682:	00 00                	add    %al,(%rax)
 684:	00 00                	add    %al,(%rax)
 686:	00 bd 08 00 00 1c    	add    %bh,0x1c000008(%rbp)
 68c:	69 11 00 00 00 00    	imul   $0x0,(%rcx),%edx
 692:	00 00                	add    %al,(%rax)
 694:	9c                   	pushfq 
 695:	08 00                	or     %al,(%rax)
 697:	00 1c 7a             	add    %bl,(%rdx,%rdi,2)
 69a:	11 00                	adc    %eax,(%rax)
 69c:	00 00                	add    %al,(%rax)
 69e:	00 00                	add    %al,(%rax)
 6a0:	00 86 08 00 00 1c    	add    %al,0x1c000008(%rsi)
 6a6:	82                   	(bad)  
 6a7:	11 00                	adc    %eax,(%rax)
 6a9:	00 00                	add    %al,(%rax)
 6ab:	00 00                	add    %al,(%rax)
 6ad:	00 c8                	add    %cl,%al
 6af:	08 00                	or     %al,(%rax)
 6b1:	00 1c 87             	add    %bl,(%rdi,%rax,4)
 6b4:	11 00                	adc    %eax,(%rax)
 6b6:	00 00                	add    %al,(%rax)
 6b8:	00 00                	add    %al,(%rax)
 6ba:	00 9c 08 00 00 1c 98 	add    %bl,-0x67e40000(%rax,%rcx,1)
 6c1:	11 00                	adc    %eax,(%rax)
 6c3:	00 00                	add    %al,(%rax)
 6c5:	00 00                	add    %al,(%rax)
 6c7:	00 86 08 00 00 1c    	add    %al,0x1c000008(%rsi)
 6cd:	a0 11 00 00 00 00 00 	movabs 0xd300000000000011,%al
 6d4:	00 d3 
 6d6:	08 00                	or     %al,(%rax)
 6d8:	00 1c a5 11 00 00 00 	add    %bl,0x11(,%riz,4)
 6df:	00 00                	add    %al,(%rax)
 6e1:	00 9c 08 00 00 1b e1 	add    %bl,-0x1ee50000(%rax,%rcx,1)
 6e8:	11 00                	adc    %eax,(%rax)
 6ea:	00 00                	add    %al,(%rax)
 6ec:	00 00                	add    %al,(%rax)
 6ee:	00 de                	add    %bl,%dh
 6f0:	08 00                	or     %al,(%rax)
 6f2:	00 fd                	add    %bh,%ch
 6f4:	06                   	(bad)  
 6f5:	00 00                	add    %al,(%rax)
 6f7:	1a 01                	sbb    (%rcx),%al
 6f9:	55                   	push   %rbp
 6fa:	01 38                	add    %edi,(%rax)
 6fc:	00 19                	add    %bl,(%rcx)
 6fe:	04 12                	add    $0x12,%al
 700:	00 00                	add    %al,(%rax)
 702:	00 00                	add    %al,(%rax)
 704:	00 00                	add    %al,(%rax)
 706:	de 08                	fimuls (%rax)
 708:	00 00                	add    %al,(%rax)
 70a:	1a 01                	sbb    (%rcx),%al
 70c:	55                   	push   %rbp
 70d:	01 38                	add    %edi,(%rax)
 70f:	00 00                	add    %al,(%rax)
 711:	06                   	(bad)  
 712:	08 88 00 00 00 1d    	or     %cl,0x1d000000(%rax)
 718:	00 00                	add    %al,(%rax)
 71a:	00 00                	add    %al,(%rax)
 71c:	02 66 62             	add    0x62(%rsi),%ah
 71f:	00 00                	add    %al,(%rax)
 721:	00 03                	add    %al,(%rbx)
 723:	34 07                	xor    $0x7,%al
 725:	00 00                	add    %al,(%rax)
 727:	1e                   	(bad)  
 728:	3f                   	(bad)  
 729:	03 00                	add    (%rax),%eax
 72b:	00 02                	add    %al,(%rdx)
 72d:	66 c3                	retw   
 72f:	02 00                	add    (%rax),%al
 731:	00 1f                	add    %bl,(%rdi)
 733:	00 20                	add    %ah,(%rax)
 735:	3e 9e                	ds sahf 
 737:	3c 57                	cmp    $0x57,%al
 739:	65 6c                	gs insb (%dx),%es:(%rdi)
 73b:	63 6f 6d             	movslq 0x6d(%rdi),%ebp
 73e:	65 20 74 6f 20       	and    %dh,%gs:0x20(%rdi,%rbp,2)
 743:	6d                   	insl   (%dx),%es:(%rdi)
 744:	79 20                	jns    766 <_init-0x66a>
 746:	66 69 65 6e 64 69    	imul   $0x6964,0x6e(%rbp),%sp
 74c:	73 68                	jae    7b6 <_init-0x61a>
 74e:	20 6c 69 74          	and    %ch,0x74(%rcx,%rbp,2)
 752:	74 6c                	je     7c0 <_init-0x610>
 754:	65 20 62 6f          	and    %ah,%gs:0x6f(%rdx)
 758:	6d                   	insl   (%dx),%es:(%rdi)
 759:	62                   	(bad)  
 75a:	2e 20 59 6f          	and    %bl,%cs:0x6f(%rcx)
 75e:	75 20                	jne    780 <_init-0x650>
 760:	68 61 76 65 20       	pushq  $0x20657661
 765:	36 20 70 68          	and    %dh,%ss:0x68(%rax)
 769:	61                   	(bad)  
 76a:	73 65                	jae    7d1 <_init-0x5ff>
 76c:	73 20                	jae    78e <_init-0x642>
 76e:	77 69                	ja     7d9 <_init-0x5f7>
 770:	74 68                	je     7da <_init-0x5f6>
 772:	0a 00                	or     (%rax),%al
 774:	21 6d 01             	and    %ebp,0x1(%rbp)
 777:	00 00                	add    %al,(%rax)
 779:	63 01                	movslq (%rcx),%eax
 77b:	00 00                	add    %al,(%rax)
 77d:	0c 00                	or     $0x0,%al
 77f:	6d                   	insl   (%dx),%es:(%rdi)
 780:	01 00                	add    %eax,(%rax)
 782:	00 20                	add    %ah,(%rax)
 784:	2f                   	(bad)  
 785:	9e                   	sahf   
 786:	2d 77 68 69 63       	sub    $0x63696877,%eax
 78b:	68 20 74 6f 20       	pushq  $0x206f7420
 790:	62                   	(bad)  
 791:	6c                   	insb   (%dx),%es:(%rdi)
 792:	6f                   	outsl  %ds:(%rsi),(%dx)
 793:	77 20                	ja     7b5 <_init-0x61b>
 795:	79 6f                	jns    806 <_init-0x5ca>
 797:	75 72                	jne    80b <_init-0x5c5>
 799:	73 65                	jae    800 <_init-0x5d0>
 79b:	6c                   	insb   (%dx),%es:(%rdi)
 79c:	66 20 75 70          	data16 and %dh,0x70(%rbp)
 7a0:	2e 20 48 61          	and    %cl,%cs:0x61(%rax)
 7a4:	76 65                	jbe    80b <_init-0x5c5>
 7a6:	20 61 20             	and    %ah,0x20(%rcx)
 7a9:	6e                   	outsb  %ds:(%rsi),(%dx)
 7aa:	69 63 65 20 64 61 79 	imul   $0x79616420,0x65(%rbx),%esp
 7b1:	21 0a                	and    %ecx,(%rdx)
 7b3:	00 20                	add    %ah,(%rax)
 7b5:	2c 9e                	sub    $0x9e,%al
 7b7:	2a 50 68             	sub    0x68(%rax),%dl
 7ba:	61                   	(bad)  
 7bb:	73 65                	jae    822 <_init-0x5ae>
 7bd:	20 31                	and    %dh,(%rcx)
 7bf:	20 64 65 66          	and    %ah,0x66(%rbp,%riz,2)
 7c3:	75 73                	jne    838 <_init-0x598>
 7c5:	65 64 2e 20 48 6f    	gs fs and %cl,%cs:0x6f(%rax)
 7cb:	77 20                	ja     7ed <_init-0x5e3>
 7cd:	61                   	(bad)  
 7ce:	62                   	(bad)  
 7cf:	6f                   	outsl  %ds:(%rsi),(%dx)
 7d0:	75 74                	jne    846 <_init-0x58a>
 7d2:	20 74 68 65          	and    %dh,0x65(%rax,%rbp,2)
 7d6:	20 6e 65             	and    %ch,0x65(%rsi)
 7d9:	78 74                	js     84f <_init-0x581>
 7db:	20 6f 6e             	and    %ch,0x6e(%rdi)
 7de:	65 3f                	gs (bad) 
 7e0:	0a 00                	or     (%rax),%al
 7e2:	20 21                	and    %ah,(%rcx)
 7e4:	9e                   	sahf   
 7e5:	1f                   	(bad)  
 7e6:	54                   	push   %rsp
 7e7:	68 61 74 27 73       	pushq  $0x73277461
 7ec:	20 6e 75             	and    %ch,0x75(%rsi)
 7ef:	6d                   	insl   (%dx),%es:(%rdi)
 7f0:	62                   	(bad)  
 7f1:	65 72 20             	gs jb  814 <_init-0x5bc>
 7f4:	32 2e                	xor    (%rsi),%ch
 7f6:	20 20                	and    %ah,(%rax)
 7f8:	4b                   	rex.WXB
 7f9:	65 65 70 20          	gs gs jo 81d <_init-0x5b3>
 7fd:	67 6f                	outsl  %ds:(%esi),(%dx)
 7ff:	69 6e 67 21 0a 00 20 	imul   $0x20000a21,0x67(%rsi),%ebp
 806:	12 9e 10 48 61 6c    	adc    0x6c614810(%rsi),%bl
 80c:	66 77 61             	data16 ja 870 <_init-0x560>
 80f:	79 20                	jns    831 <_init-0x59f>
 811:	74 68                	je     87b <_init-0x555>
 813:	65 72 65             	gs jb  87b <_init-0x555>
 816:	21 0a                	and    %ecx,(%rdx)
 818:	00 20                	add    %ah,(%rax)
 81a:	27                   	(bad)  
 81b:	9e                   	sahf   
 81c:	25 53 6f 20 79       	and    $0x79206f53,%eax
 821:	6f                   	outsl  %ds:(%rsi),(%dx)
 822:	75 20                	jne    844 <_init-0x58c>
 824:	67 6f                	outsl  %ds:(%esi),(%dx)
 826:	74 20                	je     848 <_init-0x588>
 828:	74 68                	je     892 <_init-0x53e>
 82a:	61                   	(bad)  
 82b:	74 20                	je     84d <_init-0x583>
 82d:	6f                   	outsl  %ds:(%rsi),(%dx)
 82e:	6e                   	outsb  %ds:(%rsi),(%dx)
 82f:	65 2e 20 20          	gs and %ah,%cs:(%rax)
 833:	54                   	push   %rsp
 834:	72 79                	jb     8af <_init-0x521>
 836:	20 74 68 69          	and    %dh,0x69(%rax,%rbp,2)
 83a:	73 20                	jae    85c <_init-0x574>
 83c:	6f                   	outsl  %ds:(%rsi),(%dx)
 83d:	6e                   	outsb  %ds:(%rsi),(%dx)
 83e:	65 2e 0a 00          	gs or  %cs:(%rax),%al
 842:	20 21                	and    %ah,(%rcx)
 844:	9e                   	sahf   
 845:	1f                   	(bad)  
 846:	47 6f                	rex.RXB outsl %ds:(%rsi),(%dx)
 848:	6f                   	outsl  %ds:(%rsi),(%dx)
 849:	64 20 77 6f          	and    %dh,%fs:0x6f(%rdi)
 84d:	72 6b                	jb     8ba <_init-0x516>
 84f:	21 20                	and    %esp,(%rax)
 851:	20 4f 6e             	and    %cl,0x6e(%rdi)
 854:	20 74 6f 20          	and    %dh,0x20(%rdi,%rbp,2)
 858:	74 68                	je     8c2 <_init-0x50e>
 85a:	65 20 6e 65          	and    %ch,%gs:0x65(%rsi)
 85e:	78 74                	js     8d4 <_init-0x4fc>
 860:	2e 2e 2e 0a 00       	cs cs or %cs:(%rax),%al
 865:	22 c2                	and    %dl,%al
 867:	00 00                	add    %al,(%rax)
 869:	00 c2                	add    %al,%dl
 86b:	00 00                	add    %al,(%rax)
 86d:	00 02                	add    %al,(%rdx)
 86f:	57                   	push   %rdi
 870:	22 aa 01 00 00 aa    	and    -0x55ffffff(%rdx),%ch
 876:	01 00                	add    %eax,(%rax)
 878:	00 07                	add    %al,(%rdi)
 87a:	e8 22 e2 00 00       	callq  eaa1 <__FRAME_END__+0xb8f5>
 87f:	00 e2                	add    %ah,%dl
 881:	00 00                	add    %al,(%rax)
 883:	00 09                	add    %cl,(%rcx)
 885:	09 22                	or     %esp,(%rdx)
 887:	68 00 00 00 68       	pushq  $0x68000000
 88c:	00 00                	add    %al,(%rax)
 88e:	00 09                	add    %cl,(%rcx)
 890:	08 22                	or     %ah,(%rdx)
 892:	98                   	cwtl   
 893:	02 00                	add    (%rax),%al
 895:	00 98 02 00 00 0a    	add    %bl,0xa000002(%rax)
 89b:	04 22                	add    $0x22,%al
 89d:	80 00 00             	addb   $0x0,(%rax)
 8a0:	00 80 00 00 00 09    	add    %al,0x9000000(%rax)
 8a6:	0c 22                	or     $0x22,%al
 8a8:	a0 02 00 00 a0 02 00 	movabs 0xa000002a0000002,%al
 8af:	00 0a 
 8b1:	05 22 a8 02 00       	add    $0x2a822,%eax
 8b6:	00 a8 02 00 00 0a    	add    %ch,0xa000002(%rax)
 8bc:	06                   	(bad)  
 8bd:	22 b0 02 00 00 b0    	and    -0x4ffffffe(%rax),%dh
 8c3:	02 00                	add    (%rax),%al
 8c5:	00 0a                	add    %cl,(%rdx)
 8c7:	07                   	(bad)  
 8c8:	22 b8 02 00 00 b8    	and    -0x47fffffe(%rax),%bh
 8ce:	02 00                	add    (%rax),%al
 8d0:	00 0a                	add    %cl,(%rdx)
 8d2:	08 22                	or     %ah,(%rdx)
 8d4:	c0 02 00             	rolb   $0x0,(%rdx)
 8d7:	00 c0                	add    %al,%al
 8d9:	02 00                	add    (%rax),%al
 8db:	00 0a                	add    %cl,(%rdx)
 8dd:	09 23                	or     %esp,(%rbx)
 8df:	01 02                	add    %eax,(%rdx)
 8e1:	00 00                	add    %al,(%rax)
 8e3:	01 02                	add    %eax,(%rdx)
 8e5:	00 00                	add    %al,(%rax)
 8e7:	0b 66 02             	or     0x2(%rsi),%esp
	...

Disassembly of section .debug_abbrev:

0000000000000000 <.debug_abbrev>:
   0:	01 11                	add    %edx,(%rcx)
   2:	01 25 0e 13 0b 03    	add    %esp,0x30b130e(%rip)        # 30b1316 <_end+0x2eac5e6>
   8:	0e                   	(bad)  
   9:	1b 0e                	sbb    (%rsi),%ecx
   b:	11 01                	adc    %eax,(%rcx)
   d:	12 07                	adc    (%rdi),%al
   f:	10 17                	adc    %dl,(%rdi)
  11:	00 00                	add    %al,(%rax)
  13:	02 16                	add    (%rsi),%dl
  15:	00 03                	add    %al,(%rbx)
  17:	0e                   	(bad)  
  18:	3a 0b                	cmp    (%rbx),%cl
  1a:	3b 0b                	cmp    (%rbx),%ecx
  1c:	49 13 00             	adc    (%r8),%rax
  1f:	00 03                	add    %al,(%rbx)
  21:	24 00                	and    $0x0,%al
  23:	0b 0b                	or     (%rbx),%ecx
  25:	3e 0b 03             	or     %ds:(%rbx),%eax
  28:	0e                   	(bad)  
  29:	00 00                	add    %al,(%rax)
  2b:	04 24                	add    $0x24,%al
  2d:	00 0b                	add    %cl,(%rbx)
  2f:	0b 3e                	or     (%rsi),%edi
  31:	0b 03                	or     (%rbx),%eax
  33:	08 00                	or     %al,(%rax)
  35:	00 05 0f 00 0b 0b    	add    %al,0xb0b000f(%rip)        # b0b004a <_end+0xaeab31a>
  3b:	00 00                	add    %al,(%rax)
  3d:	06                   	(bad)  
  3e:	0f 00 0b             	str    (%rbx)
  41:	0b 49 13             	or     0x13(%rcx),%ecx
  44:	00 00                	add    %al,(%rax)
  46:	07                   	(bad)  
  47:	26 00 49 13          	add    %cl,%es:0x13(%rcx)
  4b:	00 00                	add    %al,(%rax)
  4d:	08 13                	or     %dl,(%rbx)
  4f:	01 03                	add    %eax,(%rbx)
  51:	0e                   	(bad)  
  52:	0b 0b                	or     (%rbx),%ecx
  54:	3a 0b                	cmp    (%rbx),%cl
  56:	3b 0b                	cmp    (%rbx),%ecx
  58:	01 13                	add    %edx,(%rbx)
  5a:	00 00                	add    %al,(%rax)
  5c:	09 0d 00 03 0e 3a    	or     %ecx,0x3a0e0300(%rip)        # 3a0e0362 <_end+0x39edb632>
  62:	0b 3b                	or     (%rbx),%edi
  64:	0b 49 13             	or     0x13(%rcx),%ecx
  67:	38 0b                	cmp    %cl,(%rbx)
  69:	00 00                	add    %al,(%rax)
  6b:	0a 0d 00 03 0e 3a    	or     0x3a0e0300(%rip),%cl        # 3a0e0371 <_end+0x39edb641>
  71:	0b 3b                	or     (%rbx),%edi
  73:	05 49 13 38 0b       	add    $0xb381349,%eax
  78:	00 00                	add    %al,(%rax)
  7a:	0b 16                	or     (%rsi),%edx
  7c:	00 03                	add    %al,(%rbx)
  7e:	0e                   	(bad)  
  7f:	3a 0b                	cmp    (%rbx),%cl
  81:	3b 0b                	cmp    (%rbx),%ecx
  83:	00 00                	add    %al,(%rax)
  85:	0c 01                	or     $0x1,%al
  87:	01 49 13             	add    %ecx,0x13(%rcx)
  8a:	01 13                	add    %edx,(%rbx)
  8c:	00 00                	add    %al,(%rax)
  8e:	0d 21 00 49 13       	or     $0x13490021,%eax
  93:	2f                   	(bad)  
  94:	0b 00                	or     (%rax),%eax
  96:	00 0e                	add    %cl,(%rsi)
  98:	13 00                	adc    (%rax),%eax
  9a:	03 0e                	add    (%rsi),%ecx
  9c:	3c 19                	cmp    $0x19,%al
  9e:	00 00                	add    %al,(%rax)
  a0:	0f 34                	sysenter 
  a2:	00 03                	add    %al,(%rbx)
  a4:	0e                   	(bad)  
  a5:	3a 0b                	cmp    (%rbx),%cl
  a7:	3b 05 49 13 3f 19    	cmp    0x193f1349(%rip),%eax        # 193f13f6 <_end+0x191ec6c6>
  ad:	3c 19                	cmp    $0x19,%al
  af:	00 00                	add    %al,(%rax)
  b1:	10 37                	adc    %dh,(%rdi)
  b3:	00 49 13             	add    %cl,0x13(%rcx)
  b6:	00 00                	add    %al,(%rax)
  b8:	11 34 00             	adc    %esi,(%rax,%rax,1)
  bb:	03 0e                	add    (%rsi),%ecx
  bd:	3a 0b                	cmp    (%rbx),%cl
  bf:	3b 0b                	cmp    (%rbx),%ecx
  c1:	49 13 3f             	adc    (%r15),%rdi
  c4:	19 3c 19             	sbb    %edi,(%rcx,%rbx,1)
  c7:	00 00                	add    %al,(%rax)
  c9:	12 21                	adc    (%rcx),%ah
  cb:	00 00                	add    %al,(%rax)
  cd:	00 13                	add    %dl,(%rbx)
  cf:	34 00                	xor    $0x0,%al
  d1:	03 0e                	add    (%rsi),%ecx
  d3:	3a 0b                	cmp    (%rbx),%cl
  d5:	3b 0b                	cmp    (%rbx),%ecx
  d7:	49 13 3f             	adc    (%r15),%rdi
  da:	19 02                	sbb    %eax,(%rdx)
  dc:	18 00                	sbb    %al,(%rax)
  de:	00 14 2e             	add    %dl,(%rsi,%rbp,1)
  e1:	01 3f                	add    %edi,(%rdi)
  e3:	19 03                	sbb    %eax,(%rbx)
  e5:	0e                   	(bad)  
  e6:	3a 0b                	cmp    (%rbx),%cl
  e8:	3b 0b                	cmp    (%rbx),%ecx
  ea:	27                   	(bad)  
  eb:	19 49 13             	sbb    %ecx,0x13(%rcx)
  ee:	11 01                	adc    %eax,(%rcx)
  f0:	12 07                	adc    (%rdi),%al
  f2:	40 18 97 42 19 01 13 	sbb    %dl,0x13011942(%rdi)
  f9:	00 00                	add    %al,(%rax)
  fb:	15 05 00 03 0e       	adc    $0xe030005,%eax
 100:	3a 0b                	cmp    (%rbx),%cl
 102:	3b 0b                	cmp    (%rbx),%ecx
 104:	49 13 02             	adc    (%r10),%rax
 107:	17                   	(bad)  
 108:	00 00                	add    %al,(%rax)
 10a:	16                   	(bad)  
 10b:	34 00                	xor    $0x0,%al
 10d:	03 0e                	add    (%rsi),%ecx
 10f:	3a 0b                	cmp    (%rbx),%cl
 111:	3b 0b                	cmp    (%rbx),%ecx
 113:	49 13 02             	adc    (%r10),%rax
 116:	17                   	(bad)  
 117:	00 00                	add    %al,(%rax)
 119:	17                   	(bad)  
 11a:	1d 01 31 13 11       	sbb    $0x11133101,%eax
 11f:	01 12                	add    %edx,(%rdx)
 121:	07                   	(bad)  
 122:	58                   	pop    %rax
 123:	0b 59 0b             	or     0xb(%rcx),%ebx
 126:	01 13                	add    %edx,(%rbx)
 128:	00 00                	add    %al,(%rax)
 12a:	18 05 00 31 13 02    	sbb    %al,0x2133100(%rip)        # 2133230 <_end+0x1f2e500>
 130:	17                   	(bad)  
 131:	00 00                	add    %al,(%rax)
 133:	19 89 82 01 01 11    	sbb    %ecx,0x11010182(%rcx)
 139:	01 31                	add    %esi,(%rcx)
 13b:	13 00                	adc    (%rax),%eax
 13d:	00 1a                	add    %bl,(%rdx)
 13f:	8a 82 01 00 02 18    	mov    0x18020001(%rdx),%al
 145:	91                   	xchg   %eax,%ecx
 146:	42 18 00             	rex.X sbb %al,(%rax)
 149:	00 1b                	add    %bl,(%rbx)
 14b:	89 82 01 01 11 01    	mov    %eax,0x1110101(%rdx)
 151:	31 13                	xor    %edx,(%rbx)
 153:	01 13                	add    %edx,(%rbx)
 155:	00 00                	add    %al,(%rax)
 157:	1c 89                	sbb    $0x89,%al
 159:	82                   	(bad)  
 15a:	01 00                	add    %eax,(%rax)
 15c:	11 01                	adc    %eax,(%rcx)
 15e:	31 13                	xor    %edx,(%rbx)
 160:	00 00                	add    %al,(%rax)
 162:	1d 2e 01 3f 19       	sbb    $0x193f012e,%eax
 167:	03 0e                	add    (%rsi),%ecx
 169:	3a 0b                	cmp    (%rbx),%cl
 16b:	3b 0b                	cmp    (%rbx),%ecx
 16d:	27                   	(bad)  
 16e:	19 49 13             	sbb    %ecx,0x13(%rcx)
 171:	20 0b                	and    %cl,(%rbx)
 173:	34 19                	xor    $0x19,%al
 175:	01 13                	add    %edx,(%rbx)
 177:	00 00                	add    %al,(%rax)
 179:	1e                   	(bad)  
 17a:	05 00 03 0e 3a       	add    $0x3a0e0300,%eax
 17f:	0b 3b                	or     (%rbx),%edi
 181:	0b 49 13             	or     0x13(%rcx),%ecx
 184:	00 00                	add    %al,(%rax)
 186:	1f                   	(bad)  
 187:	18 00                	sbb    %al,(%rax)
 189:	00 00                	add    %al,(%rax)
 18b:	20 36                	and    %dh,(%rsi)
 18d:	00 02                	add    %al,(%rdx)
 18f:	18 00                	sbb    %al,(%rax)
 191:	00 21                	add    %ah,(%rcx)
 193:	2e 00 3f             	add    %bh,%cs:(%rdi)
 196:	19 3c 19             	sbb    %edi,(%rcx,%rbx,1)
 199:	6e                   	outsb  %ds:(%rsi),(%dx)
 19a:	0e                   	(bad)  
 19b:	03 0e                	add    (%rsi),%ecx
 19d:	3a 0b                	cmp    (%rbx),%cl
 19f:	3b 0b                	cmp    (%rbx),%ecx
 1a1:	6e                   	outsb  %ds:(%rsi),(%dx)
 1a2:	0e                   	(bad)  
 1a3:	00 00                	add    %al,(%rax)
 1a5:	22 2e                	and    (%rsi),%ch
 1a7:	00 3f                	add    %bh,(%rdi)
 1a9:	19 3c 19             	sbb    %edi,(%rcx,%rbx,1)
 1ac:	6e                   	outsb  %ds:(%rsi),(%dx)
 1ad:	0e                   	(bad)  
 1ae:	03 0e                	add    (%rsi),%ecx
 1b0:	3a 0b                	cmp    (%rbx),%cl
 1b2:	3b 0b                	cmp    (%rbx),%ecx
 1b4:	00 00                	add    %al,(%rax)
 1b6:	23 2e                	and    (%rsi),%ebp
 1b8:	00 3f                	add    %bh,(%rdi)
 1ba:	19 3c 19             	sbb    %edi,(%rcx,%rbx,1)
 1bd:	6e                   	outsb  %ds:(%rsi),(%dx)
 1be:	0e                   	(bad)  
 1bf:	03 0e                	add    (%rsi),%ecx
 1c1:	3a 0b                	cmp    (%rbx),%cl
 1c3:	3b                   	.byte 0x3b
 1c4:	05                   	.byte 0x5
 1c5:	00 00                	add    %al,(%rax)
	...

Disassembly of section .debug_line:

0000000000000000 <.debug_line>:
   0:	a2 01 00 00 02 00 25 	movabs %al,0x1250002000001
   7:	01 00 
   9:	00 01                	add    %al,(%rcx)
   b:	01 fb                	add    %edi,%ebx
   d:	0e                   	(bad)  
   e:	0d 00 01 01 01       	or     $0x1010100,%eax
  13:	01 00                	add    %eax,(%rax)
  15:	00 00                	add    %al,(%rax)
  17:	01 00                	add    %eax,(%rax)
  19:	00 01                	add    %al,(%rcx)
  1b:	2f                   	(bad)  
  1c:	75 73                	jne    91 <_init-0xd3f>
  1e:	72 2f                	jb     4f <_init-0xd81>
  20:	69 6e 63 6c 75 64 65 	imul   $0x6564756c,0x63(%rsi),%ebp
  27:	2f                   	(bad)  
  28:	78 38                	js     62 <_init-0xd6e>
  2a:	36 5f                	ss pop %rdi
  2c:	36 34 2d             	ss xor $0x2d,%al
  2f:	6c                   	insb   (%dx),%es:(%rdi)
  30:	69 6e 75 78 2d 67 6e 	imul   $0x6e672d78,0x75(%rsi),%ebp
  37:	75 2f                	jne    68 <_init-0xd68>
  39:	62                   	(bad)  
  3a:	69 74 73 00 2f 75 73 	imul   $0x7273752f,0x0(%rbx,%rsi,2),%esi
  41:	72 
  42:	2f                   	(bad)  
  43:	6c                   	insb   (%dx),%es:(%rdi)
  44:	69 62 2f 67 63 63 2f 	imul   $0x2f636367,0x2f(%rdx),%esp
  4b:	78 38                	js     85 <_init-0xd4b>
  4d:	36 5f                	ss pop %rdi
  4f:	36 34 2d             	ss xor $0x2d,%al
  52:	6c                   	insb   (%dx),%es:(%rdi)
  53:	69 6e 75 78 2d 67 6e 	imul   $0x6e672d78,0x75(%rsi),%ebp
  5a:	75 2f                	jne    8b <_init-0xd45>
  5c:	37                   	(bad)  
  5d:	2f                   	(bad)  
  5e:	69 6e 63 6c 75 64 65 	imul   $0x6564756c,0x63(%rsi),%ebp
  65:	00 2f                	add    %ch,(%rdi)
  67:	75 73                	jne    dc <_init-0xcf4>
  69:	72 2f                	jb     9a <_init-0xd36>
  6b:	69 6e 63 6c 75 64 65 	imul   $0x6564756c,0x63(%rsi),%ebp
  72:	2f                   	(bad)  
  73:	78 38                	js     ad <_init-0xd23>
  75:	36 5f                	ss pop %rdi
  77:	36 34 2d             	ss xor $0x2d,%al
  7a:	6c                   	insb   (%dx),%es:(%rdi)
  7b:	69 6e 75 78 2d 67 6e 	imul   $0x6e672d78,0x75(%rsi),%ebp
  82:	75 2f                	jne    b3 <_init-0xd1d>
  84:	62                   	(bad)  
  85:	69 74 73 2f 74 79 70 	imul   $0x65707974,0x2f(%rbx,%rsi,2),%esi
  8c:	65 
  8d:	73 00                	jae    8f <_init-0xd41>
  8f:	2f                   	(bad)  
  90:	75 73                	jne    105 <_init-0xccb>
  92:	72 2f                	jb     c3 <_init-0xd0d>
  94:	69 6e 63 6c 75 64 65 	imul   $0x6564756c,0x63(%rsi),%ebp
  9b:	00 00                	add    %al,(%rax)
  9d:	62                   	(bad)  
  9e:	6f                   	outsl  %ds:(%rsi),(%dx)
  9f:	6d                   	insl   (%dx),%es:(%rdi)
  a0:	62                   	(bad)  
  a1:	2e 63 00             	movslq %cs:(%rax),%eax
  a4:	00 00                	add    %al,(%rax)
  a6:	00 73 74             	add    %dh,0x74(%rbx)
  a9:	64 69 6f 32 2e 68 00 	imul   $0x100682e,%fs:0x32(%rdi),%ebp
  b0:	01 
  b1:	00 00                	add    %al,(%rax)
  b3:	73 74                	jae    129 <_init-0xca7>
  b5:	64 64 65 66 2e 68 00 	fs fs gs cs pushw $0x200
  bc:	02 
  bd:	00 00                	add    %al,(%rax)
  bf:	74 79                	je     13a <_init-0xc96>
  c1:	70 65                	jo     128 <_init-0xca8>
  c3:	73 2e                	jae    f3 <_init-0xcdd>
  c5:	68 00 01 00 00       	pushq  $0x100
  ca:	6c                   	insb   (%dx),%es:(%rdi)
  cb:	69 62 69 6f 2e 68 00 	imul   $0x682e6f,0x69(%rdx),%esp
  d2:	01 00                	add    %eax,(%rax)
  d4:	00 46 49             	add    %al,0x49(%rsi)
  d7:	4c                   	rex.WR
  d8:	45                   	rex.RB
  d9:	2e 68 00 03 00 00    	cs pushq $0x300
  df:	73 74                	jae    155 <_init-0xc7b>
  e1:	64 69 6f 2e 68 00 04 	imul   $0x40068,%fs:0x2e(%rdi),%ebp
  e8:	00 
  e9:	00 73 79             	add    %dh,0x79(%rbx)
  ec:	73 5f                	jae    14d <_init-0xc83>
  ee:	65 72 72             	gs jb  163 <_init-0xc6d>
  f1:	6c                   	insb   (%dx),%es:(%rdi)
  f2:	69 73 74 2e 68 00 01 	imul   $0x100682e,0x74(%rbx),%esi
  f9:	00 00                	add    %al,(%rax)
  fb:	73 75                	jae    172 <_init-0xc5e>
  fd:	70 70                	jo     16f <_init-0xc61>
  ff:	6f                   	outsl  %ds:(%rsi),(%dx)
 100:	72 74                	jb     176 <_init-0xc5a>
 102:	2e 68 00 00 00 00    	cs pushq $0x0
 108:	70 68                	jo     172 <_init-0xc5e>
 10a:	61                   	(bad)  
 10b:	73 65                	jae    172 <_init-0xc5e>
 10d:	73 2e                	jae    13d <_init-0xc93>
 10f:	68 00 00 00 00       	pushq  $0x0
 114:	73 74                	jae    18a <_init-0xc46>
 116:	64 6c                	fs insb (%dx),%es:(%rdi)
 118:	69 62 2e 68 00 04 00 	imul   $0x40068,0x2e(%rdx),%esp
 11f:	00 3c 62             	add    %bh,(%rdx,%riz,2)
 122:	75 69                	jne    18d <_init-0xc43>
 124:	6c                   	insb   (%dx),%es:(%rdi)
 125:	74 2d                	je     154 <_init-0xc7c>
 127:	69 6e 3e 00 00 00 00 	imul   $0x0,0x3e(%rsi),%ebp
 12e:	00 00                	add    %al,(%rax)
 130:	09 02                	or     %eax,(%rdx)
 132:	aa                   	stos   %al,%es:(%rdi)
 133:	10 00                	adc    %al,(%rax)
 135:	00 00                	add    %al,(%rax)
 137:	00 00                	add    %al,(%rax)
 139:	00 03                	add    %al,(%rbx)
 13b:	24 01                	and    $0x1,%al
 13d:	28 c2                	sub    %al,%dl
 13f:	91                   	xchg   %eax,%ecx
 140:	03 0d 08 e4 04 02    	add    0x204e408(%rip),%ecx        # 204e54e <_end+0x1e4981e>
 146:	03 25 58 04 01 03    	add    0x3010458(%rip),%esp        # 30105a4 <_end+0x2e0b874>
 14c:	61                   	(bad)  
 14d:	08 74 59 83          	or     %dh,-0x7d(%rcx,%rbx,2)
 151:	04 02                	add    $0x2,%al
 153:	03 1d 58 04 01 03    	add    0x3010458(%rip),%ebx        # 30105b1 <_end+0x2e0b881>
 159:	69 ba 59 83 04 02 03 	imul   $0x4581503,0x2048359(%rdx),%edi
 160:	15 58 04 
 163:	01 03                	add    %eax,(%rbx)
 165:	70 ba                	jo     121 <_init-0xcaf>
 167:	59                   	pop    %rcx
 168:	83 04 02 03          	addl   $0x3,(%rdx,%rax,1)
 16c:	0e                   	(bad)  
 16d:	58                   	pop    %rax
 16e:	04 01                	add    $0x1,%al
 170:	03 76 ba             	add    -0x46(%rsi),%esi
 173:	59                   	pop    %rcx
 174:	83 04 02 60          	addl   $0x60,(%rdx,%rax,1)
 178:	04 01                	add    $0x1,%al
 17a:	b6 59                	mov    $0x59,%dh
 17c:	83 04 02 5a          	addl   $0x5a,(%rdx,%rax,1)
 180:	04 01                	add    $0x1,%al
 182:	bd 59 83 5e 03       	mov    $0x35e8359,%ebp
 187:	bb 7f 74 04 02       	mov    $0x204747f,%ebx
 18c:	03 3a                	add    (%rdx),%edi
 18e:	08 2e                	or     %ch,(%rsi)
 190:	04 01                	add    $0x1,%al
 192:	03 50 08             	add    0x8(%rax),%edx
 195:	74 04                	je     19b <_init-0xc35>
 197:	02 03                	add    (%rbx),%al
 199:	30 9e 04 01 03 57    	xor    %bl,0x57030104(%rsi)
 19f:	08 82 02 0a 00 01    	or     %al,0x1000a02(%rdx)
 1a5:	01                   	.byte 0x1

Disassembly of section .debug_str:

0000000000000000 <.debug_str>:
   0:	70 72                	jo     74 <_init-0xd5c>
   2:	69 6e 74 66 00 5f 5f 	imul   $0x5f5f0066,0x74(%rsi),%ebp
   9:	6f                   	outsl  %ds:(%rsi),(%dx)
   a:	66 66 5f             	data16 pop %di
   d:	74 00                	je     f <_init-0xdc1>
   f:	5f                   	pop    %rdi
  10:	49                   	rex.WB
  11:	4f 5f                	rex.WRXB pop %r15
  13:	72 65                	jb     7a <_init-0xd56>
  15:	61                   	(bad)  
  16:	64 5f                	fs pop %rdi
  18:	70 74                	jo     8e <_init-0xd42>
  1a:	72 00                	jb     1c <_init-0xdb4>
  1c:	5f                   	pop    %rdi
  1d:	63 68 61             	movslq 0x61(%rax),%ebp
  20:	69 6e 00 73 69 7a 65 	imul   $0x657a6973,0x0(%rsi),%ebp
  27:	5f                   	pop    %rdi
  28:	74 00                	je     2a <_init-0xda6>
  2a:	5f                   	pop    %rdi
  2b:	73 68                	jae    95 <_init-0xd3b>
  2d:	6f                   	outsl  %ds:(%rsi),(%dx)
  2e:	72 74                	jb     a4 <_init-0xd2c>
  30:	62                   	(bad)  
  31:	75 66                	jne    99 <_init-0xd37>
  33:	00 5f 49             	add    %bl,0x49(%rdi)
  36:	4f 5f                	rex.WRXB pop %r15
  38:	32 5f 31             	xor    0x31(%rdi),%bl
  3b:	5f                   	pop    %rdi
  3c:	73 74                	jae    b2 <_init-0xd1e>
  3e:	64 65 72 72          	fs gs jb b4 <_init-0xd1c>
  42:	5f                   	pop    %rdi
  43:	00 5f 49             	add    %bl,0x49(%rdi)
  46:	4f 5f                	rex.WRXB pop %r15
  48:	62                   	(bad)  
  49:	75 66                	jne    b1 <_init-0xd1f>
  4b:	5f                   	pop    %rdi
  4c:	62 61                	(bad)  
  4e:	73 65                	jae    b5 <_init-0xd1b>
  50:	00 6c 6f 6e          	add    %ch,0x6e(%rdi,%rbp,2)
  54:	67 20 6c 6f 6e       	and    %ch,0x6e(%edi,%ebp,2)
  59:	67 20 75 6e          	and    %dh,0x6e(%ebp)
  5d:	73 69                	jae    c8 <_init-0xd08>
  5f:	67 6e                	outsb  %ds:(%esi),(%dx)
  61:	65 64 20 69 6e       	gs and %ch,%fs:0x6e(%rcx)
  66:	74 00                	je     68 <_init-0xd68>
  68:	72 65                	jb     cf <_init-0xd01>
  6a:	61                   	(bad)  
  6b:	64 5f                	fs pop %rdi
  6d:	6c                   	insb   (%dx),%es:(%rdi)
  6e:	69 6e 65 00 6c 6f 6e 	imul   $0x6e6f6c00,0x65(%rsi),%ebp
  75:	67 20 6c 6f 6e       	and    %ch,0x6e(%edi,%ebp,2)
  7a:	67 20 69 6e          	and    %ch,0x6e(%ecx)
  7e:	74 00                	je     80 <_init-0xd50>
  80:	70 68                	jo     ea <_init-0xce6>
  82:	61                   	(bad)  
  83:	73 65                	jae    ea <_init-0xce6>
  85:	5f                   	pop    %rdi
  86:	64 65 66 75 73       	fs gs data16 jne fe <_init-0xcd2>
  8b:	65 64 00 5f 66       	gs add %bl,%fs:0x66(%rdi)
  90:	69 6c 65 6e 6f 00 5f 	imul   $0x495f006f,0x6e(%rbp,%riz,2),%ebp
  97:	49 
  98:	4f 5f                	rex.WRXB pop %r15
  9a:	72 65                	jb     101 <_init-0xccf>
  9c:	61                   	(bad)  
  9d:	64 5f                	fs pop %rdi
  9f:	65 6e                	outsb  %gs:(%rsi),(%dx)
  a1:	64 00 5f 66          	add    %bl,%fs:0x66(%rdi)
  a5:	6c                   	insb   (%dx),%es:(%rdi)
  a6:	61                   	(bad)  
  a7:	67 73 00             	addr32 jae aa <_init-0xd26>
  aa:	5f                   	pop    %rdi
  ab:	49                   	rex.WB
  ac:	4f 5f                	rex.WRXB pop %r15
  ae:	62                   	(bad)  
  af:	75 66                	jne    117 <_init-0xcb9>
  b1:	5f                   	pop    %rdi
  b2:	65 6e                	outsb  %gs:(%rsi),(%dx)
  b4:	64 00 5f 63          	add    %bl,%fs:0x63(%rdi)
  b8:	75 72                	jne    12c <_init-0xca4>
  ba:	5f                   	pop    %rdi
  bb:	63 6f 6c             	movslq 0x6c(%rdi),%ebp
  be:	75 6d                	jne    12d <_init-0xca3>
  c0:	6e                   	outsb  %ds:(%rsi),(%dx)
  c1:	00 5f 5f             	add    %bl,0x5f(%rdi)
  c4:	70 72                	jo     138 <_init-0xc98>
  c6:	69 6e 74 66 5f 63 68 	imul   $0x68635f66,0x74(%rsi),%ebp
  cd:	6b 00 5f             	imul   $0x5f,(%rax),%eax
  d0:	6f                   	outsl  %ds:(%rsi),(%dx)
  d1:	6c                   	insb   (%dx),%es:(%rdi)
  d2:	64 5f                	fs pop %rdi
  d4:	6f                   	outsl  %ds:(%rsi),(%dx)
  d5:	66 66 73 65          	data16 data16 jae 13e <_init-0xc92>
  d9:	74 00                	je     db <_init-0xcf5>
  db:	69 6e 66 69 6c 65 00 	imul   $0x656c69,0x66(%rsi),%ebp
  e2:	69 6e 69 74 69 61 6c 	imul   $0x6c616974,0x69(%rsi),%ebp
  e9:	69 7a 65 5f 62 6f 6d 	imul   $0x6d6f625f,0x65(%rdx),%edi
  f0:	62                   	(bad)  
  f1:	00 5f 49             	add    %bl,0x49(%rdi)
  f4:	4f 5f                	rex.WRXB pop %r15
  f6:	6d                   	insl   (%dx),%es:(%rdi)
  f7:	61                   	(bad)  
  f8:	72 6b                	jb     165 <_init-0xc6b>
  fa:	65 72 00             	gs jb  fd <_init-0xcd3>
  fd:	73 74                	jae    173 <_init-0xc5d>
  ff:	64 69 6e 00 5f 49 4f 	imul   $0x5f4f495f,%fs:0x0(%rsi),%ebp
 106:	5f 
 107:	46                   	rex.RX
 108:	49                   	rex.WB
 109:	4c                   	rex.WR
 10a:	45 5f                	rex.RB pop %r15
 10c:	70 6c                	jo     17a <_init-0xc56>
 10e:	75 73                	jne    183 <_init-0xc4d>
 110:	00 5f 49             	add    %bl,0x49(%rdi)
 113:	4f 5f                	rex.WRXB pop %r15
 115:	77 72                	ja     189 <_init-0xc47>
 117:	69 74 65 5f 70 74 72 	imul   $0x727470,0x5f(%rbp,%riz,2),%esi
 11e:	00 
 11f:	73 79                	jae    19a <_init-0xc36>
 121:	73 5f                	jae    182 <_init-0xc4e>
 123:	6e                   	outsb  %ds:(%rsi),(%dx)
 124:	65 72 72             	gs jb  199 <_init-0xc37>
 127:	00 5f 73             	add    %bl,0x73(%rdi)
 12a:	62                   	(bad)  
 12b:	75 66                	jne    193 <_init-0xc3d>
 12d:	00 73 68             	add    %dh,0x68(%rbx)
 130:	6f                   	outsl  %ds:(%rsi),(%dx)
 131:	72 74                	jb     1a7 <_init-0xc29>
 133:	20 75 6e             	and    %dh,0x6e(%rbp)
 136:	73 69                	jae    1a1 <_init-0xc2f>
 138:	67 6e                	outsb  %ds:(%esi),(%dx)
 13a:	65 64 20 69 6e       	gs and %ch,%fs:0x6e(%rcx)
 13f:	74 00                	je     141 <_init-0xc8f>
 141:	5f                   	pop    %rdi
 142:	49                   	rex.WB
 143:	4f 5f                	rex.WRXB pop %r15
 145:	73 61                	jae    1a8 <_init-0xc28>
 147:	76 65                	jbe    1ae <_init-0xc22>
 149:	5f                   	pop    %rdi
 14a:	62 61                	(bad)  
 14c:	73 65                	jae    1b3 <_init-0xc1d>
 14e:	00 5f 6c             	add    %bl,0x6c(%rdi)
 151:	6f                   	outsl  %ds:(%rsi),(%dx)
 152:	63 6b 00             	movslq 0x0(%rbx),%ebp
 155:	5f                   	pop    %rdi
 156:	66 6c                	data16 insb (%dx),%es:(%rdi)
 158:	61                   	(bad)  
 159:	67 73 32             	addr32 jae 18e <_init-0xc42>
 15c:	00 5f 6d             	add    %bl,0x6d(%rdi)
 15f:	6f                   	outsl  %ds:(%rsi),(%dx)
 160:	64 65 00 5f 5f       	fs add %bl,%gs:0x5f(%rdi)
 165:	62                   	(bad)  
 166:	75 69                	jne    1d1 <_init-0xbff>
 168:	6c                   	insb   (%dx),%es:(%rdi)
 169:	74 69                	je     1d4 <_init-0xbfc>
 16b:	6e                   	outsb  %ds:(%rsi),(%dx)
 16c:	5f                   	pop    %rdi
 16d:	70 75                	jo     1e4 <_init-0xbec>
 16f:	74 73                	je     1e4 <_init-0xbec>
 171:	00 73 74             	add    %dh,0x74(%rbx)
 174:	64 6f                	outsl  %fs:(%rsi),(%dx)
 176:	75 74                	jne    1ec <_init-0xbe4>
 178:	00 5f 49             	add    %bl,0x49(%rdi)
 17b:	4f 5f                	rex.WRXB pop %r15
 17d:	32 5f 31             	xor    0x31(%rdi),%bl
 180:	5f                   	pop    %rdi
 181:	73 74                	jae    1f7 <_init-0xbd9>
 183:	64 69 6e 5f 00 5f 49 	imul   $0x4f495f00,%fs:0x5f(%rsi),%ebp
 18a:	4f 
 18b:	5f                   	pop    %rdi
 18c:	77 72                	ja     200 <_init-0xbd0>
 18e:	69 74 65 5f 65 6e 64 	imul   $0x646e65,0x5f(%rbp,%riz,2),%esi
 195:	00 
 196:	5f                   	pop    %rdi
 197:	49                   	rex.WB
 198:	4f 5f                	rex.WRXB pop %r15
 19a:	6c                   	insb   (%dx),%es:(%rdi)
 19b:	6f                   	outsl  %ds:(%rsi),(%dx)
 19c:	63 6b 5f             	movslq 0x5f(%rbx),%ebp
 19f:	74 00                	je     1a1 <_init-0xc2f>
 1a1:	5f                   	pop    %rdi
 1a2:	49                   	rex.WB
 1a3:	4f 5f                	rex.WRXB pop %r15
 1a5:	46                   	rex.RX
 1a6:	49                   	rex.WB
 1a7:	4c                   	rex.WR
 1a8:	45 00 66 6f          	add    %r12b,0x6f(%r14)
 1ac:	70 65                	jo     213 <_init-0xbbd>
 1ae:	6e                   	outsb  %ds:(%rsi),(%dx)
 1af:	00 5f 70             	add    %bl,0x70(%rdi)
 1b2:	6f                   	outsl  %ds:(%rsi),(%dx)
 1b3:	73 00                	jae    1b5 <_init-0xc1b>
 1b5:	73 79                	jae    230 <_init-0xba0>
 1b7:	73 5f                	jae    218 <_init-0xbb8>
 1b9:	65 72 72             	gs jb  22e <_init-0xba2>
 1bc:	6c                   	insb   (%dx),%es:(%rdi)
 1bd:	69 73 74 00 5f 6d 61 	imul   $0x616d5f00,0x74(%rbx),%esi
 1c4:	72 6b                	jb     231 <_init-0xb9f>
 1c6:	65 72 73             	gs jb  23c <_init-0xb94>
 1c9:	00 75 6e             	add    %dh,0x6e(%rbp)
 1cc:	73 69                	jae    237 <_init-0xb99>
 1ce:	67 6e                	outsb  %ds:(%esi),(%dx)
 1d0:	65 64 20 63 68       	gs and %ah,%fs:0x68(%rbx)
 1d5:	61                   	(bad)  
 1d6:	72 00                	jb     1d8 <_init-0xbf8>
 1d8:	73 68                	jae    242 <_init-0xb8e>
 1da:	6f                   	outsl  %ds:(%rsi),(%dx)
 1db:	72 74                	jb     251 <_init-0xb7f>
 1dd:	20 69 6e             	and    %ch,0x6e(%rcx)
 1e0:	74 00                	je     1e2 <_init-0xbee>
 1e2:	5f                   	pop    %rdi
 1e3:	76 74                	jbe    259 <_init-0xb77>
 1e5:	61                   	(bad)  
 1e6:	62                   	(bad)  
 1e7:	6c                   	insb   (%dx),%es:(%rdi)
 1e8:	65 5f                	gs pop %rdi
 1ea:	6f                   	outsl  %ds:(%rsi),(%dx)
 1eb:	66 66 73 65          	data16 data16 jae 254 <_init-0xb7c>
 1ef:	74 00                	je     1f1 <_init-0xbdf>
 1f1:	5f                   	pop    %rdi
 1f2:	49                   	rex.WB
 1f3:	4f 5f                	rex.WRXB pop %r15
 1f5:	32 5f 31             	xor    0x31(%rdi),%bl
 1f8:	5f                   	pop    %rdi
 1f9:	73 74                	jae    26f <_init-0xb61>
 1fb:	64 6f                	outsl  %fs:(%rsi),(%dx)
 1fd:	75 74                	jne    273 <_init-0xb5d>
 1ff:	5f                   	pop    %rdi
 200:	00 65 78             	add    %ah,0x78(%rbp)
 203:	69 74 00 62 6f 6d 62 	imul   $0x2e626d6f,0x62(%rax,%rax,1),%esi
 20a:	2e 
 20b:	63 00                	movslq (%rax),%eax
 20d:	69 6e 70 75 74 00 5f 	imul   $0x5f007475,0x70(%rsi),%ebp
 214:	6e                   	outsb  %ds:(%rsi),(%dx)
 215:	65 78 74             	gs js  28c <_init-0xb44>
 218:	00 5f 5f             	add    %bl,0x5f(%rdi)
 21b:	6f                   	outsl  %ds:(%rsi),(%dx)
 21c:	66 66 36 34 5f       	data16 data16 ss xor $0x5f,%al
 221:	74 00                	je     223 <_init-0xbad>
 223:	5f                   	pop    %rdi
 224:	49                   	rex.WB
 225:	4f 5f                	rex.WRXB pop %r15
 227:	72 65                	jb     28e <_init-0xb42>
 229:	61                   	(bad)  
 22a:	64 5f                	fs pop %rdi
 22c:	62 61                	(bad)  
 22e:	73 65                	jae    295 <_init-0xb3b>
 230:	00 5f 49             	add    %bl,0x49(%rdi)
 233:	4f 5f                	rex.WRXB pop %r15
 235:	73 61                	jae    298 <_init-0xb38>
 237:	76 65                	jbe    29e <_init-0xb32>
 239:	5f                   	pop    %rdi
 23a:	65 6e                	outsb  %gs:(%rsi),(%dx)
 23c:	64 00 2f             	add    %ch,%fs:(%rdi)
 23f:	68 6f 6d 65 2f       	pushq  $0x2f656d6f
 244:	61                   	(bad)  
 245:	67 2f                	addr32 (bad) 
 247:	44                   	rex.R
 248:	65 73 6b             	gs jae 2b6 <_init-0xb1a>
 24b:	74 6f                	je     2bc <_init-0xb14>
 24d:	70 2f                	jo     27e <_init-0xb52>
 24f:	6c                   	insb   (%dx),%es:(%rdi)
 250:	61                   	(bad)  
 251:	62 73 2f 62 6f       	(bad)  {%k2}
 256:	6d                   	insl   (%dx),%es:(%rdi)
 257:	62                   	(bad)  
 258:	6c                   	insb   (%dx),%es:(%rdi)
 259:	61                   	(bad)  
 25a:	62                   	(bad)  
 25b:	2f                   	(bad)  
 25c:	73 72                	jae    2d0 <_init-0xb00>
 25e:	63 00                	movslq (%rax),%eax
 260:	5f                   	pop    %rdi
 261:	5f                   	pop    %rdi
 262:	70 61                	jo     2c5 <_init-0xb0b>
 264:	64 31 00             	xor    %eax,%fs:(%rax)
 267:	5f                   	pop    %rdi
 268:	5f                   	pop    %rdi
 269:	70 61                	jo     2cc <_init-0xb04>
 26b:	64 32 00             	xor    %fs:(%rax),%al
 26e:	5f                   	pop    %rdi
 26f:	5f                   	pop    %rdi
 270:	70 61                	jo     2d3 <_init-0xafd>
 272:	64 33 00             	xor    %fs:(%rax),%eax
 275:	5f                   	pop    %rdi
 276:	5f                   	pop    %rdi
 277:	70 61                	jo     2da <_init-0xaf6>
 279:	64 34 00             	fs xor $0x0,%al
 27c:	5f                   	pop    %rdi
 27d:	5f                   	pop    %rdi
 27e:	70 61                	jo     2e1 <_init-0xaef>
 280:	64 35 00 5f 75 6e    	fs xor $0x6e755f00,%eax
 286:	75 73                	jne    2fb <_init-0xad5>
 288:	65 64 32 00          	gs xor %fs:(%rax),%al
 28c:	73 74                	jae    302 <_init-0xace>
 28e:	64 65 72 72          	fs gs jb 304 <_init-0xacc>
 292:	00 61 72             	add    %ah,0x72(%rcx)
 295:	67 76 00             	addr32 jbe 298 <_init-0xb38>
 298:	70 68                	jo     302 <_init-0xace>
 29a:	61                   	(bad)  
 29b:	73 65                	jae    302 <_init-0xace>
 29d:	5f                   	pop    %rdi
 29e:	31 00                	xor    %eax,(%rax)
 2a0:	70 68                	jo     30a <_init-0xac6>
 2a2:	61                   	(bad)  
 2a3:	73 65                	jae    30a <_init-0xac6>
 2a5:	5f                   	pop    %rdi
 2a6:	32 00                	xor    (%rax),%al
 2a8:	70 68                	jo     312 <_init-0xabe>
 2aa:	61                   	(bad)  
 2ab:	73 65                	jae    312 <_init-0xabe>
 2ad:	5f                   	pop    %rdi
 2ae:	33 00                	xor    (%rax),%eax
 2b0:	70 68                	jo     31a <_init-0xab6>
 2b2:	61                   	(bad)  
 2b3:	73 65                	jae    31a <_init-0xab6>
 2b5:	5f                   	pop    %rdi
 2b6:	34 00                	xor    $0x0,%al
 2b8:	70 68                	jo     322 <_init-0xaae>
 2ba:	61                   	(bad)  
 2bb:	73 65                	jae    322 <_init-0xaae>
 2bd:	5f                   	pop    %rdi
 2be:	35 00 70 68 61       	xor    $0x61687000,%eax
 2c3:	73 65                	jae    32a <_init-0xaa6>
 2c5:	5f                   	pop    %rdi
 2c6:	36 00 5f 49          	add    %bl,%ss:0x49(%rdi)
 2ca:	4f 5f                	rex.WRXB pop %r15
 2cc:	62 61                	(bad)  
 2ce:	63 6b 75             	movslq 0x75(%rbx),%ebp
 2d1:	70 5f                	jo     332 <_init-0xa9e>
 2d3:	62 61                	(bad)  
 2d5:	73 65                	jae    33c <_init-0xa94>
 2d7:	00 61 72             	add    %ah,0x72(%rcx)
 2da:	67 63 00             	movslq (%eax),%eax
 2dd:	47                   	rex.RXB
 2de:	4e 55                	rex.WRX push %rbp
 2e0:	20 43 31             	and    %al,0x31(%rbx)
 2e3:	31 20                	xor    %esp,(%rax)
 2e5:	37                   	(bad)  
 2e6:	2e 35 2e 30 20 2d    	cs xor $0x2d20302e,%eax
 2ec:	6d                   	insl   (%dx),%es:(%rdi)
 2ed:	74 75                	je     364 <_init-0xa6c>
 2ef:	6e                   	outsb  %ds:(%rsi),(%dx)
 2f0:	65 3d 67 65 6e 65    	gs cmp $0x656e6567,%eax
 2f6:	72 69                	jb     361 <_init-0xa6f>
 2f8:	63 20                	movslq (%rax),%esp
 2fa:	2d 6d 61 72 63       	sub    $0x6372616d,%eax
 2ff:	68 3d 78 38 36       	pushq  $0x3638783d
 304:	2d 36 34 20 2d       	sub    $0x2d203436,%eax
 309:	67 67 64 62          	addr32 addr32 fs (bad) 
 30d:	20 2d 4f 31 20 2d    	and    %ch,0x2d20314f(%rip)        # 2d203462 <_end+0x2cffe732>
 313:	66 73 74             	data16 jae 38a <_init-0xa46>
 316:	61                   	(bad)  
 317:	63 6b 2d             	movslq 0x2d(%rbx),%ebp
 31a:	70 72                	jo     38e <_init-0xa42>
 31c:	6f                   	outsl  %ds:(%rsi),(%dx)
 31d:	74 65                	je     384 <_init-0xa4c>
 31f:	63 74 6f 72          	movslq 0x72(%rdi,%rbp,2),%esi
 323:	2d 73 74 72 6f       	sub    $0x6f727473,%eax
 328:	6e                   	outsb  %ds:(%rsi),(%dx)
 329:	67 00 6d 61          	add    %ch,0x61(%ebp)
 32d:	69 6e 00 5f 49 4f 5f 	imul   $0x5f4f495f,0x0(%rsi),%ebp
 334:	77 72                	ja     3a8 <_init-0xa28>
 336:	69 74 65 5f 62 61 73 	imul   $0x65736162,0x5f(%rbp,%riz,2),%esi
 33d:	65 
 33e:	00 5f 5f             	add    %bl,0x5f(%rdi)
 341:	66 6d                	insw   (%dx),%es:(%rdi)
 343:	74 00                	je     345 <_init-0xa8b>

Disassembly of section .debug_loc:

0000000000000000 <.debug_loc>:
	...
   8:	1a 00                	sbb    (%rax),%al
   a:	00 00                	add    %al,(%rax)
   c:	00 00                	add    %al,(%rax)
   e:	00 00                	add    %al,(%rax)
  10:	01 00                	add    %eax,(%rax)
  12:	55                   	push   %rbp
  13:	1a 00                	sbb    (%rax),%al
  15:	00 00                	add    %al,(%rax)
  17:	00 00                	add    %al,(%rax)
  19:	00 00                	add    %al,(%rax)
  1b:	02 01                	add    (%rcx),%al
  1d:	00 00                	add    %al,(%rax)
  1f:	00 00                	add    %al,(%rax)
  21:	00 00                	add    %al,(%rax)
  23:	04 00                	add    $0x0,%al
  25:	f3 01 55 9f          	repz add %edx,-0x61(%rbp)
  29:	02 01                	add    (%rcx),%al
  2b:	00 00                	add    %al,(%rax)
  2d:	00 00                	add    %al,(%rax)
  2f:	00 00                	add    %al,(%rax)
  31:	15 01 00 00 00       	adc    $0x1,%eax
  36:	00 00                	add    %al,(%rax)
  38:	00 01                	add    %al,(%rcx)
  3a:	00 55 15             	add    %dl,0x15(%rbp)
  3d:	01 00                	add    %eax,(%rax)
  3f:	00 00                	add    %al,(%rax)
  41:	00 00                	add    %al,(%rax)
  43:	00 37                	add    %dh,(%rdi)
  45:	01 00                	add    %eax,(%rax)
  47:	00 00                	add    %al,(%rax)
  49:	00 00                	add    %al,(%rax)
  4b:	00 04 00             	add    %al,(%rax,%rax,1)
  4e:	f3 01 55 9f          	repz add %edx,-0x61(%rbp)
  52:	37                   	(bad)  
  53:	01 00                	add    %eax,(%rax)
  55:	00 00                	add    %al,(%rax)
  57:	00 00                	add    %al,(%rax)
  59:	00 46 01             	add    %al,0x1(%rsi)
  5c:	00 00                	add    %al,(%rax)
  5e:	00 00                	add    %al,(%rax)
  60:	00 00                	add    %al,(%rax)
  62:	01 00                	add    %eax,(%rax)
  64:	55                   	push   %rbp
  65:	46 01 00             	rex.RX add %r8d,(%rax)
  68:	00 00                	add    %al,(%rax)
  6a:	00 00                	add    %al,(%rax)
  6c:	00 5a 01             	add    %bl,0x1(%rdx)
  6f:	00 00                	add    %al,(%rax)
  71:	00 00                	add    %al,(%rax)
  73:	00 00                	add    %al,(%rax)
  75:	04 00                	add    $0x0,%al
  77:	f3 01 55 9f          	repz add %edx,-0x61(%rbp)
	...
  93:	21 00                	and    %eax,(%rax)
  95:	00 00                	add    %al,(%rax)
  97:	00 00                	add    %al,(%rax)
  99:	00 00                	add    %al,(%rax)
  9b:	01 00                	add    %eax,(%rax)
  9d:	54                   	push   %rsp
  9e:	21 00                	and    %eax,(%rax)
  a0:	00 00                	add    %al,(%rax)
  a2:	00 00                	add    %al,(%rax)
  a4:	00 00                	add    %al,(%rax)
  a6:	36 00 00             	add    %al,%ss:(%rax)
  a9:	00 00                	add    %al,(%rax)
  ab:	00 00                	add    %al,(%rax)
  ad:	00 01                	add    %al,(%rcx)
  af:	00 53 36             	add    %dl,0x36(%rbx)
  b2:	00 00                	add    %al,(%rax)
  b4:	00 00                	add    %al,(%rax)
  b6:	00 00                	add    %al,(%rax)
  b8:	00 02                	add    %al,(%rdx)
  ba:	01 00                	add    %eax,(%rax)
  bc:	00 00                	add    %al,(%rax)
  be:	00 00                	add    %al,(%rax)
  c0:	00 04 00             	add    %al,(%rax,%rax,1)
  c3:	f3 01 54 9f 02       	repz add %edx,0x2(%rdi,%rbx,4)
  c8:	01 00                	add    %eax,(%rax)
  ca:	00 00                	add    %al,(%rax)
  cc:	00 00                	add    %al,(%rax)
  ce:	00 15 01 00 00 00    	add    %dl,0x1(%rip)        # d5 <_init-0xcfb>
  d4:	00 00                	add    %al,(%rax)
  d6:	00 01                	add    %al,(%rcx)
  d8:	00 54 15 01          	add    %dl,0x1(%rbp,%rdx,1)
  dc:	00 00                	add    %al,(%rax)
  de:	00 00                	add    %al,(%rax)
  e0:	00 00                	add    %al,(%rax)
  e2:	5a                   	pop    %rdx
  e3:	01 00                	add    %eax,(%rax)
  e5:	00 00                	add    %al,(%rax)
  e7:	00 00                	add    %al,(%rax)
  e9:	00 01                	add    %al,(%rcx)
  eb:	00 53 00             	add    %dl,0x0(%rbx)
	...
  fa:	00 00                	add    %al,(%rax)
  fc:	00 58 00             	add    %bl,0x0(%rax)
  ff:	00 00                	add    %al,(%rax)
 101:	00 00                	add    %al,(%rax)
 103:	00 00                	add    %al,(%rax)
 105:	5f                   	pop    %rdi
 106:	00 00                	add    %al,(%rax)
 108:	00 00                	add    %al,(%rax)
 10a:	00 00                	add    %al,(%rax)
 10c:	00 01                	add    %al,(%rcx)
 10e:	00 50 76             	add    %dl,0x76(%rax)
 111:	00 00                	add    %al,(%rax)
 113:	00 00                	add    %al,(%rax)
 115:	00 00                	add    %al,(%rax)
 117:	00 7d 00             	add    %bh,0x0(%rbp)
 11a:	00 00                	add    %al,(%rax)
 11c:	00 00                	add    %al,(%rax)
 11e:	00 00                	add    %al,(%rax)
 120:	01 00                	add    %eax,(%rax)
 122:	50                   	push   %rax
 123:	94                   	xchg   %eax,%esp
 124:	00 00                	add    %al,(%rax)
 126:	00 00                	add    %al,(%rax)
 128:	00 00                	add    %al,(%rax)
 12a:	00 9b 00 00 00 00    	add    %bl,0x0(%rbx)
 130:	00 00                	add    %al,(%rax)
 132:	00 01                	add    %al,(%rcx)
 134:	00 50 b2             	add    %dl,-0x4e(%rax)
 137:	00 00                	add    %al,(%rax)
 139:	00 00                	add    %al,(%rax)
 13b:	00 00                	add    %al,(%rax)
 13d:	00 b9 00 00 00 00    	add    %bh,0x0(%rcx)
 143:	00 00                	add    %al,(%rax)
 145:	00 01                	add    %al,(%rcx)
 147:	00 50 d0             	add    %dl,-0x30(%rax)
 14a:	00 00                	add    %al,(%rax)
 14c:	00 00                	add    %al,(%rax)
 14e:	00 00                	add    %al,(%rax)
 150:	00 d7                	add    %dl,%bh
 152:	00 00                	add    %al,(%rax)
 154:	00 00                	add    %al,(%rax)
 156:	00 00                	add    %al,(%rax)
 158:	00 01                	add    %al,(%rcx)
 15a:	00 50 ee             	add    %dl,-0x12(%rax)
 15d:	00 00                	add    %al,(%rax)
 15f:	00 00                	add    %al,(%rax)
 161:	00 00                	add    %al,(%rax)
 163:	00 f5                	add    %dh,%ch
 165:	00 00                	add    %al,(%rax)
 167:	00 00                	add    %al,(%rax)
 169:	00 00                	add    %al,(%rax)
 16b:	00 01                	add    %al,(%rcx)
 16d:	00 50 00             	add    %dl,0x0(%rax)
	...
 17c:	00 00                	add    %al,(%rax)
 17e:	00 3b                	add    %bh,(%rbx)
 180:	00 00                	add    %al,(%rax)
 182:	00 00                	add    %al,(%rax)
 184:	00 00                	add    %al,(%rax)
 186:	00 47 00             	add    %al,0x0(%rdi)
 189:	00 00                	add    %al,(%rax)
 18b:	00 00                	add    %al,(%rax)
 18d:	00 00                	add    %al,(%rax)
 18f:	06                   	(bad)  
 190:	00 f2                	add    %dh,%dl
 192:	34 07                	xor    $0x7,%al
	...
 1a4:	00 00                	add    %al,(%rax)
 1a6:	00 47 00             	add    %al,0x0(%rdi)
 1a9:	00 00                	add    %al,(%rax)
 1ab:	00 00                	add    %al,(%rax)
 1ad:	00 00                	add    %al,(%rax)
 1af:	53                   	push   %rbx
 1b0:	00 00                	add    %al,(%rax)
 1b2:	00 00                	add    %al,(%rax)
 1b4:	00 00                	add    %al,(%rax)
 1b6:	00 06                	add    %al,(%rsi)
 1b8:	00 f2                	add    %dh,%dl
 1ba:	83 07 00             	addl   $0x0,(%rdi)
	...
 1cd:	00 00                	add    %al,(%rax)
 1cf:	65 00 00             	add    %al,%gs:(%rax)
 1d2:	00 00                	add    %al,(%rax)
 1d4:	00 00                	add    %al,(%rax)
 1d6:	00 71 00             	add    %dh,0x0(%rcx)
 1d9:	00 00                	add    %al,(%rax)
 1db:	00 00                	add    %al,(%rax)
 1dd:	00 00                	add    %al,(%rax)
 1df:	06                   	(bad)  
 1e0:	00 f2                	add    %dh,%dl
 1e2:	b4 07                	mov    $0x7,%ah
	...
 1f4:	00 00                	add    %al,(%rax)
 1f6:	00 83 00 00 00 00    	add    %al,0x0(%rbx)
 1fc:	00 00                	add    %al,(%rax)
 1fe:	00 8f 00 00 00 00    	add    %cl,0x0(%rdi)
 204:	00 00                	add    %al,(%rax)
 206:	00 06                	add    %al,(%rsi)
 208:	00 f2                	add    %dh,%dl
 20a:	e2 07                	loop   213 <_init-0xbbd>
	...
 21c:	00 00                	add    %al,(%rax)
 21e:	00 a1 00 00 00 00    	add    %ah,0x0(%rcx)
 224:	00 00                	add    %al,(%rax)
 226:	00 ad 00 00 00 00    	add    %ch,0x0(%rbp)
 22c:	00 00                	add    %al,(%rax)
 22e:	00 06                	add    %al,(%rsi)
 230:	00 f2                	add    %dh,%dl
 232:	05 08 00 00 00       	add    $0x8,%eax
	...
 247:	bf 00 00 00 00       	mov    $0x0,%edi
 24c:	00 00                	add    %al,(%rax)
 24e:	00 cb                	add    %cl,%bl
 250:	00 00                	add    %al,(%rax)
 252:	00 00                	add    %al,(%rax)
 254:	00 00                	add    %al,(%rax)
 256:	00 06                	add    %al,(%rsi)
 258:	00 f2                	add    %dh,%dl
 25a:	19 08                	sbb    %ecx,(%rax)
	...
 26c:	00 00                	add    %al,(%rax)
 26e:	00 dd                	add    %bl,%ch
 270:	00 00                	add    %al,(%rax)
 272:	00 00                	add    %al,(%rax)
 274:	00 00                	add    %al,(%rax)
 276:	00 e9                	add    %ch,%cl
 278:	00 00                	add    %al,(%rax)
 27a:	00 00                	add    %al,(%rax)
 27c:	00 00                	add    %al,(%rax)
 27e:	00 06                	add    %al,(%rsi)
 280:	00 f2                	add    %dh,%dl
 282:	42 08 00             	rex.X or %al,(%rax)
	...
 295:	00 00                	add    %al,(%rax)
 297:	15 01 00 00 00       	adc    $0x1,%eax
 29c:	00 00                	add    %al,(%rax)
 29e:	00 2d 01 00 00 00    	add    %ch,0x1(%rip)        # 2a5 <_init-0xb2b>
 2a4:	00 00                	add    %al,(%rax)
 2a6:	00 0a                	add    %cl,(%rdx)
 2a8:	00 03                	add    %al,(%rbx)
 2aa:	a6                   	cmpsb  %es:(%rdi),%ds:(%rsi)
 2ab:	26 00 00             	add    %al,%es:(%rax)
 2ae:	00 00                	add    %al,(%rax)
 2b0:	00 00                	add    %al,(%rax)
 2b2:	9f                   	lahf   
	...
 2c3:	37                   	(bad)  
 2c4:	01 00                	add    %eax,(%rax)
 2c6:	00 00                	add    %al,(%rax)
 2c8:	00 00                	add    %al,(%rax)
 2ca:	00 50 01             	add    %dl,0x1(%rax)
 2cd:	00 00                	add    %al,(%rax)
 2cf:	00 00                	add    %al,(%rax)
 2d1:	00 00                	add    %al,(%rax)
 2d3:	0a 00                	or     (%rax),%al
 2d5:	03 c3                	add    %ebx,%eax
 2d7:	26 00 00             	add    %al,%es:(%rax)
 2da:	00 00                	add    %al,(%rax)
 2dc:	00 00                	add    %al,(%rax)
 2de:	9f                   	lahf   
	...
