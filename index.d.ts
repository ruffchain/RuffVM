export class Script {
    setSandbox(sandbox: any): this;
    setOption(option: any): this;
    setUserCode(str: string): this;
    runAsyn(): Promise<void>;
}

export function createScript(type: string): Script
