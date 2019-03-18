export class Script {
    setSandbox(sandbox: any): this;
    setOption(option: any): this;
    setUserCode(str: string): this;
    runAsync(): Promise<void>;
}

export function createScript(type: string): Script
