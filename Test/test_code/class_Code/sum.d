
sum:     file format elf64-x86-64


Disassembly of section .init:

00000000000005a8 <_init>:
 5a8:	48 83 ec 08          	sub    $0x8,%rsp
 5ac:	48 8b 05 35 0a 20 00 	mov    0x200a35(%rip),%rax        # 200fe8 <__gmon_start__>
 5b3:	48 85 c0             	test   %rax,%rax
 5b6:	74 02                	je     5ba <_init+0x12>
 5b8:	ff d0                	callq  *%rax
 5ba:	48 83 c4 08          	add    $0x8,%rsp
 5be:	c3                   	retq   

Disassembly of section .plt:

00000000000005c0 <.plt>:
 5c0:	ff 35 ea 09 20 00    	pushq  0x2009ea(%rip)        # 200fb0 <_GLOBAL_OFFSET_TABLE_+0x8>
 5c6:	ff 25 ec 09 20 00    	jmpq   *0x2009ec(%rip)        # 200fb8 <_GLOBAL_OFFSET_TABLE_+0x10>
 5cc:	0f 1f 40 00          	nopl   0x0(%rax)

00000000000005d0 <__stack_chk_fail@plt>:
 5d0:	ff 25 ea 09 20 00    	jmpq   *0x2009ea(%rip)        # 200fc0 <__stack_chk_fail@GLIBC_2.4>
 5d6:	68 00 00 00 00       	pushq  $0x0
 5db:	e9 e0 ff ff ff       	jmpq   5c0 <.plt>

00000000000005e0 <strtol@plt>:
 5e0:	ff 25 e2 09 20 00    	jmpq   *0x2009e2(%rip)        # 200fc8 <strtol@GLIBC_2.2.5>
 5e6:	68 01 00 00 00       	pushq  $0x1
 5eb:	e9 d0 ff ff ff       	jmpq   5c0 <.plt>

00000000000005f0 <__printf_chk@plt>:
 5f0:	ff 25 da 09 20 00    	jmpq   *0x2009da(%rip)        # 200fd0 <__printf_chk@GLIBC_2.3.4>
 5f6:	68 02 00 00 00       	pushq  $0x2
 5fb:	e9 c0 ff ff ff       	jmpq   5c0 <.plt>

Disassembly of section .plt.got:

0000000000000600 <__cxa_finalize@plt>:
 600:	ff 25 f2 09 20 00    	jmpq   *0x2009f2(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 606:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

0000000000000610 <_start>:
 610:	31 ed                	xor    %ebp,%ebp
 612:	49 89 d1             	mov    %rdx,%r9
 615:	5e                   	pop    %rsi
 616:	48 89 e2             	mov    %rsp,%rdx
 619:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
 61d:	50                   	push   %rax
 61e:	54                   	push   %rsp
 61f:	4c 8d 05 1a 02 00 00 	lea    0x21a(%rip),%r8        # 840 <__libc_csu_fini>
 626:	48 8d 0d a3 01 00 00 	lea    0x1a3(%rip),%rcx        # 7d0 <__libc_csu_init>
 62d:	48 8d 3d f9 00 00 00 	lea    0xf9(%rip),%rdi        # 72d <main>
 634:	ff 15 a6 09 20 00    	callq  *0x2009a6(%rip)        # 200fe0 <__libc_start_main@GLIBC_2.2.5>
 63a:	f4                   	hlt    
 63b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000000640 <deregister_tm_clones>:
 640:	48 8d 3d c9 09 20 00 	lea    0x2009c9(%rip),%rdi        # 201010 <__TMC_END__>
 647:	55                   	push   %rbp
 648:	48 8d 05 c1 09 20 00 	lea    0x2009c1(%rip),%rax        # 201010 <__TMC_END__>
 64f:	48 39 f8             	cmp    %rdi,%rax
 652:	48 89 e5             	mov    %rsp,%rbp
 655:	74 19                	je     670 <deregister_tm_clones+0x30>
 657:	48 8b 05 7a 09 20 00 	mov    0x20097a(%rip),%rax        # 200fd8 <_ITM_deregisterTMCloneTable>
 65e:	48 85 c0             	test   %rax,%rax
 661:	74 0d                	je     670 <deregister_tm_clones+0x30>
 663:	5d                   	pop    %rbp
 664:	ff e0                	jmpq   *%rax
 666:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 66d:	00 00 00 
 670:	5d                   	pop    %rbp
 671:	c3                   	retq   
 672:	0f 1f 40 00          	nopl   0x0(%rax)
 676:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 67d:	00 00 00 

0000000000000680 <register_tm_clones>:
 680:	48 8d 3d 89 09 20 00 	lea    0x200989(%rip),%rdi        # 201010 <__TMC_END__>
 687:	48 8d 35 82 09 20 00 	lea    0x200982(%rip),%rsi        # 201010 <__TMC_END__>
 68e:	55                   	push   %rbp
 68f:	48 29 fe             	sub    %rdi,%rsi
 692:	48 89 e5             	mov    %rsp,%rbp
 695:	48 c1 fe 03          	sar    $0x3,%rsi
 699:	48 89 f0             	mov    %rsi,%rax
 69c:	48 c1 e8 3f          	shr    $0x3f,%rax
 6a0:	48 01 c6             	add    %rax,%rsi
 6a3:	48 d1 fe             	sar    %rsi
 6a6:	74 18                	je     6c0 <register_tm_clones+0x40>
 6a8:	48 8b 05 41 09 20 00 	mov    0x200941(%rip),%rax        # 200ff0 <_ITM_registerTMCloneTable>
 6af:	48 85 c0             	test   %rax,%rax
 6b2:	74 0c                	je     6c0 <register_tm_clones+0x40>
 6b4:	5d                   	pop    %rbp
 6b5:	ff e0                	jmpq   *%rax
 6b7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
 6be:	00 00 
 6c0:	5d                   	pop    %rbp
 6c1:	c3                   	retq   
 6c2:	0f 1f 40 00          	nopl   0x0(%rax)
 6c6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 6cd:	00 00 00 

00000000000006d0 <__do_global_dtors_aux>:
 6d0:	80 3d 39 09 20 00 00 	cmpb   $0x0,0x200939(%rip)        # 201010 <__TMC_END__>
 6d7:	75 2f                	jne    708 <__do_global_dtors_aux+0x38>
 6d9:	48 83 3d 17 09 20 00 	cmpq   $0x0,0x200917(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 6e0:	00 
 6e1:	55                   	push   %rbp
 6e2:	48 89 e5             	mov    %rsp,%rbp
 6e5:	74 0c                	je     6f3 <__do_global_dtors_aux+0x23>
 6e7:	48 8b 3d 1a 09 20 00 	mov    0x20091a(%rip),%rdi        # 201008 <__dso_handle>
 6ee:	e8 0d ff ff ff       	callq  600 <__cxa_finalize@plt>
 6f3:	e8 48 ff ff ff       	callq  640 <deregister_tm_clones>
 6f8:	c6 05 11 09 20 00 01 	movb   $0x1,0x200911(%rip)        # 201010 <__TMC_END__>
 6ff:	5d                   	pop    %rbp
 700:	c3                   	retq   
 701:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 708:	f3 c3                	repz retq 
 70a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000000710 <frame_dummy>:
 710:	55                   	push   %rbp
 711:	48 89 e5             	mov    %rsp,%rbp
 714:	5d                   	pop    %rbp
 715:	e9 66 ff ff ff       	jmpq   680 <register_tm_clones>

000000000000071a <plus>:
 71a:	48 8d 04 37          	lea    (%rdi,%rsi,1),%rax
 71e:	c3                   	retq   

000000000000071f <sumstore>:
 71f:	53                   	push   %rbx
 720:	48 89 d3             	mov    %rdx,%rbx
 723:	e8 f2 ff ff ff       	callq  71a <plus>
 728:	48 89 03             	mov    %rax,(%rbx)
 72b:	5b                   	pop    %rbx
 72c:	c3                   	retq   

000000000000072d <main>:
 72d:	55                   	push   %rbp
 72e:	53                   	push   %rbx
 72f:	48 83 ec 18          	sub    $0x18,%rsp
 733:	48 89 f5             	mov    %rsi,%rbp
 736:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
 73d:	00 00 
 73f:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
 744:	31 c0                	xor    %eax,%eax
 746:	48 8b 7e 08          	mov    0x8(%rsi),%rdi
 74a:	ba 0a 00 00 00       	mov    $0xa,%edx
 74f:	be 00 00 00 00       	mov    $0x0,%esi
 754:	e8 87 fe ff ff       	callq  5e0 <strtol@plt>
 759:	48 63 d8             	movslq %eax,%rbx
 75c:	48 8b 7d 10          	mov    0x10(%rbp),%rdi
 760:	ba 0a 00 00 00       	mov    $0xa,%edx
 765:	be 00 00 00 00       	mov    $0x0,%esi
 76a:	e8 71 fe ff ff       	callq  5e0 <strtol@plt>
 76f:	48 63 e8             	movslq %eax,%rbp
 772:	48 89 e2             	mov    %rsp,%rdx
 775:	48 89 ee             	mov    %rbp,%rsi
 778:	48 89 df             	mov    %rbx,%rdi
 77b:	e8 9f ff ff ff       	callq  71f <sumstore>
 780:	4c 8b 04 24          	mov    (%rsp),%r8
 784:	48 89 e9             	mov    %rbp,%rcx
 787:	48 89 da             	mov    %rbx,%rdx
 78a:	48 8d 35 c3 00 00 00 	lea    0xc3(%rip),%rsi        # 854 <_IO_stdin_used+0x4>
 791:	bf 01 00 00 00       	mov    $0x1,%edi
 796:	b8 00 00 00 00       	mov    $0x0,%eax
 79b:	e8 50 fe ff ff       	callq  5f0 <__printf_chk@plt>
 7a0:	48 8b 4c 24 08       	mov    0x8(%rsp),%rcx
 7a5:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
 7ac:	00 00 
 7ae:	75 0c                	jne    7bc <main+0x8f>
 7b0:	b8 00 00 00 00       	mov    $0x0,%eax
 7b5:	48 83 c4 18          	add    $0x18,%rsp
 7b9:	5b                   	pop    %rbx
 7ba:	5d                   	pop    %rbp
 7bb:	c3                   	retq   
 7bc:	e8 0f fe ff ff       	callq  5d0 <__stack_chk_fail@plt>
 7c1:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 7c8:	00 00 00 
 7cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000007d0 <__libc_csu_init>:
 7d0:	41 57                	push   %r15
 7d2:	41 56                	push   %r14
 7d4:	49 89 d7             	mov    %rdx,%r15
 7d7:	41 55                	push   %r13
 7d9:	41 54                	push   %r12
 7db:	4c 8d 25 c6 05 20 00 	lea    0x2005c6(%rip),%r12        # 200da8 <__frame_dummy_init_array_entry>
 7e2:	55                   	push   %rbp
 7e3:	48 8d 2d c6 05 20 00 	lea    0x2005c6(%rip),%rbp        # 200db0 <__init_array_end>
 7ea:	53                   	push   %rbx
 7eb:	41 89 fd             	mov    %edi,%r13d
 7ee:	49 89 f6             	mov    %rsi,%r14
 7f1:	4c 29 e5             	sub    %r12,%rbp
 7f4:	48 83 ec 08          	sub    $0x8,%rsp
 7f8:	48 c1 fd 03          	sar    $0x3,%rbp
 7fc:	e8 a7 fd ff ff       	callq  5a8 <_init>
 801:	48 85 ed             	test   %rbp,%rbp
 804:	74 20                	je     826 <__libc_csu_init+0x56>
 806:	31 db                	xor    %ebx,%ebx
 808:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
 80f:	00 
 810:	4c 89 fa             	mov    %r15,%rdx
 813:	4c 89 f6             	mov    %r14,%rsi
 816:	44 89 ef             	mov    %r13d,%edi
 819:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
 81d:	48 83 c3 01          	add    $0x1,%rbx
 821:	48 39 dd             	cmp    %rbx,%rbp
 824:	75 ea                	jne    810 <__libc_csu_init+0x40>
 826:	48 83 c4 08          	add    $0x8,%rsp
 82a:	5b                   	pop    %rbx
 82b:	5d                   	pop    %rbp
 82c:	41 5c                	pop    %r12
 82e:	41 5d                	pop    %r13
 830:	41 5e                	pop    %r14
 832:	41 5f                	pop    %r15
 834:	c3                   	retq   
 835:	90                   	nop
 836:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 83d:	00 00 00 

0000000000000840 <__libc_csu_fini>:
 840:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000000844 <_fini>:
 844:	48 83 ec 08          	sub    $0x8,%rsp
 848:	48 83 c4 08          	add    $0x8,%rsp
 84c:	c3                   	retq   
